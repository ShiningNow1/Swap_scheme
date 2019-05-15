//
// Created by ypf on 18-11-29.
//

#include "nvm.h"
#include "segment.h"
#include "f2fs.h"

/**
 * 将mpt cache拷贝到mapping区域
 * @param sbi
 */
void nvm_flush_mpt_pages(struct f2fs_sb_info *sbi, int flush_all) {
    struct nvm_sb_info *nsbi = sbi->nsbi;

    int mpt_pgoff = 0;//找到的脏页偏移
    int dirty = 0;//是否有脏位
    void *dst_addr;//dst_page映射的虚拟地址，拷贝时候的地址
    void *src_addr;//mpt cache中开始拷贝的地址
    struct page *mpt_page;

    spin_lock(&nsbi->mpt_lock);
    //如果设置flush_all，无条件刷回所有MPT page
    if (flush_all) {
        mpt_pgoff = 0;
    } else {
        mpt_pgoff = find_next_bit(nsbi->mpt_dirty_map, nsbi->mpt_dirty_map_bits, 0);
    }

    while (mpt_pgoff != nsbi->mpt_dirty_map_bits) {
        ///设置NVM是否为脏标志位
        if (!dirty) {
            nvm_debug(NVM_DEBUG, "flush mpt");
            dirty = 1;
            //设置标志位：nvm映射表为脏
            nsbi->nvm_flag |= NVM_NSB_DIRTY;
        }
        //计算mpt cache中开始拷贝的地址
        src_addr = nsbi->mpt + (PAGE_SIZE * mpt_pgoff / sizeof(unsigned int));

        //根据偏移得到要写的MPT page:有效的那一个，返回锁定的page
        mpt_page = get_next_mpt_page(sbi, mpt_pgoff);
        dst_addr = page_address(mpt_page);
        memcpy(dst_addr, src_addr, PAGE_SIZE);
        if (!PageDirty(mpt_page))
            set_page_dirty(mpt_page);
        f2fs_put_page(mpt_page, 1);

        //处理完清除脏位
        clear_bit(mpt_pgoff, nsbi->mpt_dirty_map);
        //处理下一个
        if (flush_all) {
            ++mpt_pgoff;
        } else {
            mpt_pgoff = find_next_bit(nsbi->mpt_dirty_map, nsbi->mpt_dirty_map_bits, ++mpt_pgoff);
        }
    }
    spin_unlock(&nsbi->mpt_lock);

}

//获取下一个要使用的有效mpt page（函数主要选定双份seg中有效的那个）,返回锁定的page
struct page *get_next_mpt_page(struct f2fs_sb_info *sbi, int mpt_pgoff) {
    struct nvm_sb_info *nsbi = sbi->nsbi;

    struct page *dst_page;//实际要写的mapping中page地址
    pgoff_t block_addr;//实际写的page对应硬盘的块号，也是META区域的偏移

    block_addr = nsbi->nsb->mpt_blkaddr + mpt_pgoff;
    ///根据版本位图决定返回双份seg哪一个seg中的地址
    if (!f2fs_test_bit(mpt_pgoff, nsbi->mpt_ver_map))
        block_addr += sbi->blocks_per_seg;
    //翻转位，切换版本（设置为当前使用的版本）
    f2fs_change_bit(mpt_pgoff, sbi->nsbi->mpt_ver_map);
    //获取要写的page，这里只获取内存page，因为我们要拷贝进去的是最新数据
    dst_page = f2fs_grab_meta_page(sbi, block_addr);
    return dst_page;
}

/**
 * 将cache中nsb刷回mapping page
 * @param sbi
 */
void nvm_flush_nsb_pages(struct f2fs_sb_info *sbi, pgoff_t block_addr) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    struct page *dst_page;//实际要写的mapping中page地址
    struct nvm_super_block *dst_addr;
    int mpt_ver_map_bytes = (nsbi->nsb->mpt_ver_map_bits - 1) / BITS_PER_BYTE + 1;//mpt版本位图占用字节数
    int segment_map_bytes = (nsbi->nsb->main_segment_nums - 1) / BITS_PER_BYTE + 1;//segment位图占用字节数

    //读取page
    dst_page = f2fs_grab_meta_page(sbi, block_addr);
    dst_addr = page_address(dst_page);
    //拷贝nsb固定长度
    memcpy(dst_addr, nsbi->nsb, sizeof(struct nvm_super_block));
    //拷贝mpt版本位图
    memcpy(&dst_addr->map[0], nsbi->mpt_ver_map, mpt_ver_map_bytes);
    //拷贝segment位图
    memcpy(&dst_addr->map[0] + mpt_ver_map_bytes, nsbi->segment_map, segment_map_bytes);

    if (!PageDirty(dst_page))
        set_page_dirty(dst_page);
    f2fs_put_page(dst_page, 1);
}

void nvm_half_lfu_count(struct f2fs_sb_info *sbi) {
    int i = 0;
    int nums = sbi->nsbi->nsb->main_segment_nums;
    for (; i < nums; i++) {
        //+1保证有映射的段计数不为0
        int now = atomic_read(&sbi->nsbi->lfu_count[i]);
        if (now < 0) {
            atomic_set(&sbi->nsbi->lfu_count[i], INT_MAX >> 1);
            continue;
        }
        atomic_set(&sbi->nsbi->lfu_count[i], (now + 1) >> 1);
    }
    nvm_debug(NVM_DEBUG, "nvm_half_lfu_count");
}

/**
 * 将cache中lfu刷回mapping page
 * @param sbi：
 * @param flag: 表示lfu的在双份中的哪一个中,0表示在第一份中，1表示在第二份中
 */
void nvm_flush_lfu_pages(struct f2fs_sb_info *sbi, int flag) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    int i;
    int lfu_blkaddr;// lfu区域的起始和结束块地址
    void *dst_addr;//dst_page映射的虚拟地址，拷贝时候的地址
    void *src_addr;//mpt cache中开始拷贝的地址
    struct page *lfu_page;

    int lfu_blk_num = (nsbi->nsb->main_segment_nums * sizeof(atomic_t) - 1) / PAGE_SIZE + 1;

    if (flag & CP_LFU_VER_FLAG) {
        lfu_blkaddr = nsbi->nsb->lfu_blkaddr1;
    } else {
        lfu_blkaddr = nsbi->nsb->lfu_blkaddr0;
    }

    for (i = 0; i < lfu_blk_num; ++i) {
        //计算lfu cache中开始拷贝的地址
        src_addr = nsbi->lfu_count + (PAGE_SIZE * i / sizeof(atomic_t));
        //根据物理块地址，得到要写的lfu page
        lfu_page = f2fs_grab_meta_page(sbi, lfu_blkaddr);
        dst_addr = page_address(lfu_page);
        memcpy(dst_addr, src_addr, PAGE_SIZE);

        if (!PageDirty(lfu_page))
            set_page_dirty(lfu_page);
        f2fs_put_page(lfu_page, 1);
        lfu_blkaddr++;
    }
}

/**
 * submit_bio之前，进行bio重定向
 * @param sbi
 * @param bio
 * @return
 */
struct bio *nvm_redirect_bio_test(struct f2fs_sb_info *sbi, struct bio *bio, enum page_type type) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    int nr_nvm_main_segs;
    block_t ssd_blk_addr;
    unsigned int ssd_segno;
    block_t nvm_blk_addr;

    unsigned int ssd_main_segoff, nvm_main_segoff;//查找的段号在ssd和nvm MAIN区域的偏移量
    unsigned int offset;//块在所在seg中的偏移
    int index;
    int redirect_flag;

    ssd_blk_addr = SECTOR_TO_BLOCK(bio->bi_iter.bi_sector);
    nr_nvm_main_segs = nsbi->nsb->main_segment_nums;
    ssd_segno = GET_SEGNO_FROM_SEG0(sbi, ssd_blk_addr);
    //segment在ssd MAIN区域的偏移量
    ssd_main_segoff = ssd_segno - GET_SEGNO_FROM_SEG0(sbi, sbi->raw_super->main_blkaddr);
    index = nr_nvm_main_segs + ssd_main_segoff;
    offset = GET_BLKOFF_FROM_SEG0(sbi, ssd_blk_addr);

    //重定向标志,如果读写都不需要重定向，那么这个mpt项没有映射
    redirect_flag = get_redirect_flag(sbi->nsbi, index);

    //根据页面类型判断是否是读写META区域数据
    if (type == META || type == META_FLUSH) {
        //如果标志第一次挂载，读不重定向，写重定向；
        //非第一次挂载，重定向到nvm设备中
        if (!(nsbi->nvm_flag & NVM_FIRST_MOUNR) || !is_read_io(bio_op(bio))) {
            //设置读写设备为NVM
            bio_set_dev(bio, nsbi->nbdev);
            if (is_read_io(bio_op(bio))) {
                nsbi->r_re_meta_bio_num++;// 读META区域bio数目加1
                nsbi->r_re_meta_page_num += bio->bi_vcnt;// 读META区域page数目加bio->bi_vcnt;
            } else {
                nsbi->w_re_meta_bio_num++;// 写META区域bio数目加1
                nsbi->w_re_meta_page_num += bio->bi_vcnt;// 写META区域page数目加bio->bi_vcnt;
            }
        }
        goto out;
    }


    //是否是读io
    if (is_read_io(bio_op(bio))) {
        //判断读是否需要重定向
        if (!((redirect_flag >> RED_READ) & 0x1)) {
            /* 统计非重定向的读操作 */
            // 非重定向读DATA
            if (type == DATA) {
                nsbi->r_data_bio_num++;
                nsbi->r_data_page_num += bio->bi_vcnt;
            }
            // 非重定向读NODE
            if (type == NODE) {
                nsbi->r_node_bio_num++;
                nsbi->r_node_page_num += bio->bi_vcnt;
            }
            goto out;
        }

        // nvm_debug(NVM_DEBUG, "redirect_flag:%d", redirect_flag);
    } else {
        //判断写是否需要重定向
        if (!((redirect_flag >> RED_WRITE) & 0x1)) {
            /* 统计非重定向的写操作 */
            // 非重定向写DATA
            if (type == DATA) {
                nsbi->w_data_bio_num++;
                nsbi->w_data_page_num += bio->bi_vcnt;
            }
            // 非重定向写NODE
            if (type == NODE) {
                nsbi->w_node_bio_num++;
                nsbi->w_node_page_num += bio->bi_vcnt;
            }
            goto out;
        }
        // 遇到NVM-GC段迁移时，等待NVM-GC段迁移完成，然后再提交写bio
        if (redirect_flag == 1) {
            nvm_debug(NVM_INFO, "redirect_flag = 1, ssd_segoff=%d", ssd_main_segoff);
            mutex_lock(&nsbi->nvm_gc_curseg_lock);
            mutex_unlock(&nsbi->nvm_gc_curseg_lock);
        }
        nvm_assert(redirect_flag != 1);
    }

    // 统计重定向DATA
    if (type == DATA) {
        if (is_read_io(bio_op(bio))) {
            // 重定向读DATA
            nsbi->r_re_data_bio_num++;
            nsbi->r_re_data_page_num += bio->bi_vcnt;
        } else {
            // 重定向写DATA
            nsbi->w_re_data_bio_num++;
            nsbi->w_re_data_page_num += bio->bi_vcnt;
        }
    }

    // 统计重定向NODE
    if (type == NODE) {
        if (is_read_io(bio_op(bio))) {
            // 重定向读NODE
            nsbi->r_re_node_bio_num++;
            nsbi->r_re_node_page_num += bio->bi_vcnt;
        } else {
            // 重定向写NODE
            nsbi->w_re_node_bio_num++;
            nsbi->w_re_node_page_num += bio->bi_vcnt;
        }
    }

    //segment在nvm MAIN区域的偏移量
    nvm_main_segoff = get_mpt_value(sbi->nsbi, index);
    //nvm MAIN首地址+nvm中seg偏移地址+当前seg中偏移
    nvm_blk_addr = nsbi->nsb->main_blkaddr + nvm_main_segoff * sbi->blocks_per_seg + offset;
    //设置bio读写地址
    bio->bi_iter.bi_sector = SECTOR_FROM_BLOCK(nvm_blk_addr);
    //设置读写设备为NVM
    bio_set_dev(bio, nsbi->nbdev);

    //增加NVM段访问计数
    atomic_inc(&nsbi->lfu_count[nvm_main_segoff]);

    out:
    /*
    unsigned long r_re_data_bio_num;
    unsigned long r_re_data_page_num;
    unsigned long r_re_node_bio_num;
    unsigned long r_re_node_page_num;
    unsigned long r_data_bio_num;
    unsigned long r_data_page_num;
    unsigned long r_node_bio_num;
    unsigned long r_node_page_num;

    unsigned long w_re_data_bio_num;
    unsigned long w_re_data_page_num;
    unsigned long w_re_node_bio_num;
    unsigned long w_re_node_page_num;
    unsigned long w_data_bio_num;
    unsigned long w_data_page_num;
    unsigned long w_node_bio_num;
    unsigned long w_node_page_num;

    unsigned long r_re_meta_bio_num;
    unsigned long r_re_meta_page_num;
    unsigned long w_re_meta_bio_num;
    unsigned long w_re_meta_page_num;
    */
    /*nvm_debug(NVM_INFO, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", nsbi->r_re_data_bio_num,
    nsbi->r_re_data_page_num, nsbi->r_re_node_bio_num,nsbi->r_re_node_page_num,nsbi->r_data_bio_num,nsbi->r_data_page_num,nsbi->r_node_bio_num,nsbi->r_node_page_num,
              nsbi->w_re_data_bio_num,nsbi->w_re_data_page_num,nsbi->w_re_node_bio_num,nsbi->w_re_node_page_num,nsbi->w_data_bio_num,nsbi->w_data_page_num,
              nsbi->w_node_bio_num,nsbi->w_node_page_num,nsbi->r_re_meta_bio_num,nsbi->r_re_meta_page_num,nsbi->w_re_meta_bio_num,nsbi->w_re_meta_page_num);*/
    return bio;
}

/**
 * submit_bio之前，进行bio重定向
 * @param sbi
 * @param bio
 * @return
 */
struct bio *nvm_redirect_bio(struct f2fs_sb_info *sbi, struct bio *bio, enum page_type type) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    int nr_nvm_main_segs;
    block_t ssd_blk_addr;
    unsigned int ssd_segno;
    block_t nvm_blk_addr;

    unsigned int ssd_main_segoff, nvm_main_segoff;//查找的段号在ssd和nvm MAIN区域的偏移量
    unsigned int offset;//块在所在seg中的偏移
    int index;
    int redirect_flag;

    //根据页面类型判断是否是读写META区域数据
    if (type == META || type == META_FLUSH) {
        //如果标志第一次挂载，读不重定向，写重定向；
        //非第一次挂载，重定向到nvm设备中
        if (!(nsbi->nvm_flag & NVM_FIRST_MOUNR) || !is_read_io(bio_op(bio))) {
            //设置读写设备为NVM
            bio_set_dev(bio, nsbi->nbdev);
        }
        goto out;
    }
    ssd_blk_addr = SECTOR_TO_BLOCK(bio->bi_iter.bi_sector);
    nr_nvm_main_segs = nsbi->nsb->main_segment_nums;
    ssd_segno = GET_SEGNO_FROM_SEG0(sbi, ssd_blk_addr);
    //segment在ssd MAIN区域的偏移量
    ssd_main_segoff = ssd_segno - GET_SEGNO_FROM_SEG0(sbi, sbi->raw_super->main_blkaddr);
    index = nr_nvm_main_segs + ssd_main_segoff;
    offset = GET_BLKOFF_FROM_SEG0(sbi, ssd_blk_addr);

    //重定向标志,如果读写都不需要重定向，那么这个mpt项没有映射
    redirect_flag = get_redirect_flag(sbi->nsbi, index);
    if (redirect_flag) {
        nvm_main_segoff = get_mpt_value(sbi->nsbi, index);
        nvm_debug(NVM_DEBUG,
                  "nvm_redirect_bio,read=%d,flag=%d,nvm_segoff=%d,ssd_segoff=%d,bio_st_ssd_blk=%ld,bio_ed_ssd_blk=%ld,seg_rem=%d#",
                  is_read_io(bio_op(bio)), redirect_flag, nvm_main_segoff, ssd_main_segoff, ssd_blk_addr,
                  ssd_blk_addr + bio->bi_vcnt, sbi->blocks_per_seg - (offset + bio->bi_vcnt));
    } else {
        nvm_debug(NVM_DEBUG,
                  "nvm_redirect_bio,read=%d,flag=%d,ssd_segoff=%d,bio_st_ssd_blk=%ld,bio_ed_ssd_blk=%ld,seg_rem=%d#",
                  is_read_io(bio_op(bio)), redirect_flag, ssd_main_segoff, ssd_blk_addr, ssd_blk_addr + bio->bi_vcnt,
                  sbi->blocks_per_seg - (offset + bio->bi_vcnt));
    }

    //是否是读io
    if (is_read_io(bio_op(bio))) {
        //判断读是否需要重定向
        if (!((redirect_flag >> RED_READ) & 0x1))
            goto out;
        // nvm_debug(NVM_DEBUG, "redirect_flag:%d", redirect_flag);
    } else {
        //判断写是否需要重定向
        if (!((redirect_flag >> RED_WRITE) & 0x1))
            goto out;
        // 遇到NVM-GC段迁移时，等待NVM-GC段迁移完成，然后再提交写bio
        if (redirect_flag == 1) {
            nvm_debug(NVM_INFO, "redirect_flag = 1, ssd_segoff=%d", ssd_main_segoff);
            mutex_lock(&nsbi->nvm_gc_curseg_lock);
            mutex_unlock(&nsbi->nvm_gc_curseg_lock);
        }
        nvm_assert(redirect_flag != 1);
    }
    //segment在nvm MAIN区域的偏移量
    nvm_main_segoff = get_mpt_value(sbi->nsbi, index);
    //nvm MAIN首地址+nvm中seg偏移地址+当前seg中偏移
    nvm_blk_addr = nsbi->nsb->main_blkaddr + nvm_main_segoff * sbi->blocks_per_seg + offset;
    //设置bio读写地址
    bio->bi_iter.bi_sector = SECTOR_FROM_BLOCK(nvm_blk_addr);
    //设置读写设备为NVM
    bio_set_dev(bio, nsbi->nbdev);

    //增加NVM段访问计数
    atomic_inc(&nsbi->lfu_count[nvm_main_segoff]);

    out:
    return bio;
}

int nvm_redirect_dio(struct f2fs_sb_info *sbi, struct buffer_head *bh) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    block_t ssd_blk_addr, nvm_blk_addr;
    unsigned int ssd_segno, ssd_main_segoff, nvm_main_segoff;//查找的段号在ssd和nvm MAIN区域的偏移量
    unsigned int offset;//块在所在seg中的偏移
    int index;
    int redirect_flag;// 重定向标志
    int nr_nvm_main_segs;// NVM设备MAIN区域段数目

    ssd_blk_addr = bh->b_blocknr;
    nr_nvm_main_segs = nsbi->nsb->main_segment_nums;
    ssd_segno = GET_SEGNO_FROM_SEG0(sbi, ssd_blk_addr);
    //segment在ssd MAIN区域的偏移量
    ssd_main_segoff = ssd_segno - GET_SEGNO_FROM_SEG0(sbi, sbi->raw_super->main_blkaddr);
    index = nr_nvm_main_segs + ssd_main_segoff;
    offset = GET_BLKOFF_FROM_SEG0(sbi, ssd_blk_addr);
    nvm_debug(NVM_DEBUG, "start directIO_redirect_flag");

    //重定向标志,如果读写都不需要重定向，那么这个mpt项没有映射
    redirect_flag = get_redirect_flag(sbi->nsbi, index);
    if (redirect_flag) {
        nvm_main_segoff = get_mpt_value(sbi->nsbi, index);
        // nvm_debug(NVM_DEBUG,
        //           "nvm_redirect_dio,read=%d,flag=%d,nvm_segoff=%d,ssd_segoff=%d,bio_st_ssd_blk=%ld,bio_ed_ssd_blk=%ld,seg_rem=%d#",
        //           is_read_io(bio_op(bio)), redirect_flag, nvm_main_segoff, ssd_main_segoff, ssd_blk_addr,
        //           ssd_blk_addr + bio->bi_vcnt, sbi->blocks_per_seg - (offset + bio->bi_vcnt));
    } else {
        // nvm_debug(NVM_DEBUG,
        //           "nvm_redirect_dio,read=%d,flag=%d,ssd_segoff=%d,bio_st_ssd_blk=%ld,bio_ed_ssd_blk=%ld,seg_rem=%d#",
        //           is_read_io(bio_op(bio)), redirect_flag, ssd_main_segoff, ssd_blk_addr, ssd_blk_addr + bio->bi_vcnt,
        //           sbi->blocks_per_seg - (offset + bio->bi_vcnt));
        goto out;
    }

    //是否是读io
    // if (is_read_io(bio_op(bio))) {
    //     //判断读是否需要重定向
    //     if (!((redirect_flag >> RED_READ) & 0x1))
    //         goto out;
    //     // nvm_debug(NVM_DEBUG, "redirect_flag:%d", redirect_flag);
    // } else {
    //     //判断写是否需要重定向
    //     if (!((redirect_flag >> RED_WRITE) & 0x1))
    //         goto out;
    //     // 做直接IO时，获取了inode的GC信号量，所以此处不会发生GC的问题
    //     // nvm_assert(redirect_flag != 1);
    // }
    //segment在nvm MAIN区域的偏移量
    nvm_main_segoff = get_mpt_value(sbi->nsbi, index);
    nvm_debug(NVM_DEBUG, "directIO_redirect_flag:%d, ssd_segoff=%d, nvm_segoff=%d", redirect_flag, ssd_main_segoff,
              nvm_main_segoff);
    //nvm MAIN首地址+nvm中seg偏移地址+当前seg中偏移
    nvm_blk_addr = nsbi->nsb->main_blkaddr + nvm_main_segoff * sbi->blocks_per_seg + offset;
    //设置bh读写地址
    bh->b_blocknr = nvm_blk_addr;
    //设置bh的读写设备为NVM
    bh->b_bdev = nsbi->nbdev;
    //增加NVM段访问计数
    atomic_inc(&nsbi->lfu_count[nvm_main_segoff]);
    out:
    return 0;
}

int nvm_redirect_dio_test(struct f2fs_sb_info *sbi, struct buffer_head *bh) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    block_t ssd_blk_addr, nvm_blk_addr;
    unsigned int ssd_segno, ssd_main_segoff, nvm_main_segoff;//查找的段号在ssd和nvm MAIN区域的偏移量
    unsigned int offset;//块在所在seg中的偏移
    int index;
    int redirect_flag;// 重定向标志
    int nr_nvm_main_segs;// NVM设备MAIN区域段数目

    ssd_blk_addr = bh->b_blocknr;
    nr_nvm_main_segs = nsbi->nsb->main_segment_nums;
    ssd_segno = GET_SEGNO_FROM_SEG0(sbi, ssd_blk_addr);
    //segment在ssd MAIN区域的偏移量
    ssd_main_segoff = ssd_segno - GET_SEGNO_FROM_SEG0(sbi, sbi->raw_super->main_blkaddr);
    index = nr_nvm_main_segs + ssd_main_segoff;
    offset = GET_BLKOFF_FROM_SEG0(sbi, ssd_blk_addr);
    nvm_debug(NVM_DEBUG, "start directIO_redirect_flag");

    //重定向标志,如果读写都不需要重定向，那么这个mpt项没有映射
    redirect_flag = get_redirect_flag(sbi->nsbi, index);


    if (is_read_io(bio_op(bio))) {
        // 读不重定向
        if (!((redirect_flag >> RED_READ) & 0x1)){
            nsbi-> r_not_re_page_num++;
            goto out;
        }
        // 读重定向
        nsbi-> r_re_page_num++;
    } else {
        // 写不重定向
        if (!((redirect_flag >> RED_WRITE) & 0x1)){
            nsbi->w_not_re_page_num++;
            goto out;
        }
        // 写重定向
        nsbi->w_re_page_num++;
    }
    //segment在nvm MAIN区域的偏移量
    nvm_main_segoff = get_mpt_value(sbi->nsbi, index);
    nvm_debug(NVM_DEBUG, "directIO_redirect_flag:%d, ssd_segoff=%d, nvm_segoff=%d", redirect_flag, ssd_main_segoff,
              nvm_main_segoff);
    //nvm MAIN首地址+nvm中seg偏移地址+当前seg中偏移
    nvm_blk_addr = nsbi->nsb->main_blkaddr + nvm_main_segoff * sbi->blocks_per_seg + offset;
    //设置bh读写地址
    bh->b_blocknr = nvm_blk_addr;
    //设置bh的读写设备为NVM
    bh->b_bdev = nsbi->nbdev;
    //增加NVM段访问计数
    atomic_inc(&nsbi->lfu_count[nvm_main_segoff]);
    out:
    return 0;
}

/**
 * 从nvm分配块并建立映射关系
 * @param sbi
 * @param ssd_segno
 * @return
 */
int nvm_allocate_segment(struct f2fs_sb_info *sbi, unsigned int ssd_segoff) {

    struct nvm_sb_info *nsbi = sbi->nsbi;
    unsigned int nvm_segoff = nsbi->cur_alloc_nvm_segoff;
    unsigned int entries_per_blk = F2FS_BLKSIZE / sizeof(unsigned int);//mpt区域每个块能保存多少项
    int dirty_bit[2];//表示设置的两个脏的映射项在mpt区域的页偏移
    /**查找并设置segment位图，新建segment*/
    spin_lock(&nsbi->segment_map_lock);
    if (nsbi->nsb->main_segment_free_nums == 0) {
        nvm_debug(NVM_ERR, "ERROR:nvm no space!");
        spin_unlock(&nsbi->segment_map_lock);
        return 0;
    }
//    nvm_debug(1, "begin alloc nvm seg");
    nvm_assert(nsbi->nsb->main_segment_free_nums <= nsbi->nsb->main_segment_nums);

    // nvm_debug(NVM_DEBUG,"nvm free:%d",nsbi->nsb->main_segment_free_nums);
    //得到空闲位：得到的是segment号相对于MAIN区域首部的偏移
    next:
    nvm_segoff = find_next_zero_bit(nsbi->segment_map, nsbi->nsb->main_segment_nums, nvm_segoff);
    if (nvm_segoff >= nsbi->nsb->main_segment_nums) {
        nvm_assert(nsbi->nsb->main_segment_free_nums > 0);
        nvm_segoff = 0;
        goto next;
        // nvm_debug(1, "ERROR:nvm no space!");
        // spin_unlock(&nsbi->segment_map_lock);
        // return 1;//TODO:状态标志
    }
    //FIXME:崩溃时位图一致性保证
//    if (f2fs_test_bit(nvm_segoff, (char *) nsbi->ckpt_segment_map)) {
//        ++nvm_segoff;
//        goto next;
//    }
    //设置位为1，并将空闲数减一
    if (!test_and_set_bit(nvm_segoff, nsbi->segment_map)) {
        --nsbi->nsb->main_segment_free_nums;
        sbi->nsbi->nvm_flag |= NVM_NSB_DIRTY;// 设置nsb为脏
    }
    spin_unlock(&nsbi->segment_map_lock);



    /**建立映射关系到mpt*/
    spin_lock(&nsbi->mpt_lock);
    //更新当前NVM段分配的位置
    nsbi->cur_alloc_nvm_segoff = nvm_segoff + 1;
    //建立nvm映射项
    set_mpt_entry(sbi->nsbi, nvm_segoff, ssd_segoff, (0x1 << NR_FLAGS) - 0x1);
    //建立ssd映射项
    set_mpt_entry(sbi->nsbi, nsbi->nsb->main_segment_nums + ssd_segoff, nvm_segoff, (0x1 << NR_FLAGS) - 0x1);
    /**设置脏的映射页偏移，也就是mpt_dirty_map下标*/
    dirty_bit[0] = nvm_segoff / entries_per_blk;
    test_and_set_bit(dirty_bit[0], nsbi->mpt_dirty_map);
    dirty_bit[1] = (nsbi->nsb->main_segment_nums + ssd_segoff) / entries_per_blk;
    test_and_set_bit(dirty_bit[1], nsbi->mpt_dirty_map);
    spin_unlock(&nsbi->mpt_lock);
    nvm_debug(NVM_DEBUG, "Build mapping[nvm(%d)-ssd(%d)]", nvm_segoff, ssd_segoff);

    return 0;
}

//cp结束时设置ckpt_segment_map
void nvm_set_ckpt_segment_map(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    memcpy(nsbi->ckpt_segment_map, nsbi->segment_map, (nsbi->nsb->main_segment_nums - 1) / BITS_PER_BYTE + 1);
}

/**
 * 调试函数，不直接调用，通过nvm_debug函数调用
 * @param level：调试等级
 * @param file
 * @param func
 * @param line
 * @param fmt：输出信息
 * @param ...
 */
void __nvm_debug(int level, const char *func,
                 unsigned int line, const char *fmt, ...) {
    struct va_format vaf;
    va_list args;

    if (level >= __NVM_DEBUG__)
        return;
    va_start(args, fmt);
    vaf.fmt = fmt;
    vaf.va = &args;
    printk(KERN_ERR "(%s, %u): %pV", func, line, &vaf);
    va_end(args);
}

int read_meta_page_from_SSD(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
//	struct nvm_super_block *nsb = nsbi->nsb;

    /* 起始位置的逻辑块号，结束位置的逻辑块号
        范围：从CP区域到main区域开始
    */
    unsigned long start = sbi->raw_super->cp_blkaddr;
    unsigned long end = sbi->raw_super->main_blkaddr;

    /* 将META区域读取到mapping中 */
    for (; start < end; start++) {
        struct page *page;

        /* 从磁盘读取索引块号为start的块，读到内存的page中 */
        page = f2fs_get_meta_page(sbi, start);
        //增加引用计数
//        get_page(page);
        //设置脏页，用于回写
        if (!PageDirty(page))
            set_page_dirty(page);
        f2fs_put_page(page, 1);
    }
    //取消设置nsbi->nvm_flag标志位
    nvm_assert(nsbi->nvm_flag & NVM_FIRST_MOUNR);
    nsbi->nvm_flag ^= NVM_FIRST_MOUNR;
    return 0;
}

int flush_meta_page_to_NVM(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    struct nvm_super_block *nsb = nsbi->nsb;

    /* 起始位置的逻辑块号，结束位置的逻辑块号 */
    unsigned long start = 0;
    unsigned long end = nsb->main_blkaddr;

    for (; start < end; start++) {
        struct page *page;
        page = f2fs_get_meta_page(sbi, start);
        put_page(page);  //减少引用计数

        /* 脏页写回NVM */
        f2fs_do_write_meta_page(sbi, page, FS_META_IO);
        /*或者f2fs_write_meta_pages 一次写回多个META page*/

        /* 清除脏页标志 */
        if (PageDirty(page))
            ClearPageDirty(page);
    }
    return 0;
}


/*
 * 读取 NVM 超级块结构
 * Read f2fs raw super block in NVM.
 * Because we have two copies of super block, so read both of them
 * to get the first valid one. If any one of them is broken, we pass
 * them recovery flag back to the caller.
 */
int read_nvm_super_block(struct f2fs_sb_info *sbi, int *recovery) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    int block;
    struct page *page;
    struct nvm_super_block *super;
    struct nvm_super_block *nsb_page;
    int mpt_ver_map_bytes;
    int segment_map_bytes;
    int lfu_len;
    int err = 0;

    super = kzalloc(sizeof(struct nvm_super_block), GFP_KERNEL);
    if (!super)
        return -ENOMEM;
    //TODO:此处要根据cp包中的记录来决定使用哪个nsb，需要修改
    block = sbi->ckpt->ckpt_flags & CP_NSB_VER_FLAG ? 1 : 0;
    //通过buffer_head读NVM超级块
    page = f2fs_get_meta_page(sbi, block);
    if (!page) {
        nvm_debug(NVM_ERR, "Unable to read %dth superblock in NVM", block + 1);
        err = -EIO;
    }
    nsb_page = page_address(page);
    /* sanity checking of raw super in NVM*/
    if (sanity_check_nvm_super(sbi, nsb_page)) {
        nvm_debug(NVM_ERR, "Can't find valid F2FS filesystem in %dth superblock in NVM",
                  block + 1);
        err = -EINVAL;
        /* Fail to read nvm superblock*/
        *recovery = 1;
        f2fs_put_page(page, 1);
        kfree(super);
        return err;
    }

    /* f2fs_super_block：有一个1024个字节的偏移
       nvm_super_block:没有偏移
    */
    memcpy(super, nsb_page, sizeof(*super));

    /* 初始化nvm_sb_info的字段 */
    if (init_nvm_sb_info(sbi, super)) {
        nvm_debug(NVM_ERR, "Can't init nvm sb info");
        err = -EINVAL;
        f2fs_put_page(page, 1);
        return err;
    }

    mpt_ver_map_bytes = (nsbi->nsb->mpt_ver_map_bits - 1) / BITS_PER_BYTE + 1;  // mpt版本位图占用字节数
    segment_map_bytes = (nsbi->nsb->main_segment_nums - 1) / BITS_PER_BYTE + 1; // segment位图占用字节数
    lfu_len = le32_to_cpu(nsbi->nsb->main_segment_nums); // NVM的LFU段访问计数列表中元素的个数
    nvm_debug(NVM_DEBUG, "read nsb from disk, blkno:%d", block);
    nvm_debug(NVM_DEBUG, "lfu_len:%d", lfu_len);

    /* 获得MPT版本位图和NVM segment位图 */
    memcpy(nsbi->mpt_ver_map, &nsb_page->map[0], mpt_ver_map_bytes);
    memcpy(nsbi->segment_map, &nsb_page->map[0] + mpt_ver_map_bytes, segment_map_bytes);
    memcpy(nsbi->ckpt_segment_map, nsbi->segment_map, segment_map_bytes);

    f2fs_put_page(page, 1);

    return err;
}

/*
 * 检查NVM超级块
 */
int sanity_check_nvm_super(struct f2fs_sb_info *sbi,
                           struct nvm_super_block *nsb) {
    struct super_block *sb = sbi->sb;

    /* 判断nsb的uuid与fsb的uuid是否一致 */
    if (strncmp(nsb->uuid, F2FS_RAW_SUPER(sbi)->uuid, sizeof(nsb->uuid))) {
        f2fs_msg(sb, KERN_ERR,
                 "uuid Mismatch, nvm(%s) - ssd(%s)",
                 (nsb->uuid), (F2FS_RAW_SUPER(sbi)->uuid));
        return 1;
    }


    return 0;
}

//依据nsb设置nsbi的相关字段
unsigned int init_nvm_sb_info(struct f2fs_sb_info *sbi, struct nvm_super_block *nsb) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    nsbi->nsb = nsb;

    /* 1、为MPT版本位图申请空间,单位：字节 */
    nsbi->mpt_ver_map = f2fs_kvzalloc(sbi, f2fs_bitmap_size(nsb->mpt_ver_map_bits), GFP_KERNEL);
    /* 2、为main区域segment位图申请空间 */
    nsbi->segment_map = f2fs_kvzalloc(sbi, f2fs_bitmap_size(nsb->main_segment_nums), GFP_KERNEL);
    nsbi->ckpt_segment_map = f2fs_kvzalloc(sbi, f2fs_bitmap_size(nsb->main_segment_nums), GFP_KERNEL);
    /* 3、为MPT表脏页位图申请空间 */
    /* MPT脏页位图大小 和 MPT版本位图大小 相同 */
    nsbi->mpt_dirty_map_bits = nsb->mpt_ver_map_bits;
    nsbi->mpt_dirty_map = f2fs_kvzalloc(sbi, f2fs_bitmap_size(nsbi->mpt_dirty_map_bits), GFP_KERNEL);

    if (!(nsbi->mpt_ver_map && nsbi->segment_map && nsbi->mpt_dirty_map)) {
        return -ENOMEM;
    }

    mutex_init(&nsbi->nvmgc_mutex);

    spin_lock_init(&nsbi->mpt_ver_map_lock);
    spin_lock_init(&nsbi->segment_map_lock);
    spin_lock_init(&nsbi->mpt_lock);

    /* 为MPT cache分配内存空间，为了读取和写回的方便，分配整数page大小的MPT cache */
    nsbi->mpt = f2fs_kvzalloc(sbi, ((nsb->mpt_entries * sizeof(unsigned int) - 1) / PAGE_SIZE + 1) * PAGE_SIZE,
                              GFP_KERNEL);
    /* 4.为lfu_count cache分配内存空间，为了对NVM段进行访问计数，方便对NVM段进行垃圾回收 */
    nsbi->lfu_count = f2fs_kvzalloc(sbi, ((nsb->main_segment_nums * sizeof(atomic_t) - 1) / PAGE_SIZE + 1) * PAGE_SIZE,
                                    GFP_KERNEL);
    nsbi->mh = init_max_heap(sbi, NVM_LFU_FREE);
    nsbi->nvm_gc_start_page = alloc_pages(GFP_KERNEL, sbi->log_blocks_per_seg);// 分配用于NVM-GC段迁移的连续物理内存页
    nvm_assert(nsbi->nvm_gc_start_page);


    if (!nsbi->lfu_count || !nsbi->mpt)
        return -ENOMEM;

    return 0;
}

//获取当前使用的有效mpt page（函数主要选定双份seg中有效的那个）
struct page *get_current_mpt_page(struct f2fs_sb_info *sbi, int mpt_pgoff) {
    struct nvm_sb_info *nsbi = sbi->nsbi;

    struct page *dst_page;//当前有效的mapping中page地址
    pgoff_t block_addr;//实际读的page对应硬盘的块号，也是META区域的偏移

    block_addr = nsbi->nsb->mpt_blkaddr + mpt_pgoff;
    ///根据版本位图决定返回双份seg哪一个seg中的地址
    if (f2fs_test_bit(mpt_pgoff, nsbi->mpt_ver_map))
        block_addr += sbi->blocks_per_seg;

    //获取要写的page，这里获取实际的page内容
    dst_page = f2fs_get_meta_page(sbi, block_addr);
    return dst_page;
}


//读取MPT映射表，将mpt表保存到cache中
void f2fs_get_mpt(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    struct nvm_super_block *nsb = nsbi->nsb;
    struct page *mpt_page;
    int i;
    void *dst_addr;
    void *src_addr;

    for (i = 0; i < nsb->mpt_ver_map_bits; ++i) {
        ///根据MPT版本位图得到当前有效的MPT page
        mpt_page = get_current_mpt_page(sbi, i);

        src_addr = page_address(mpt_page);
        dst_addr = nsbi->mpt + (PAGE_SIZE * i) / sizeof(unsigned int);

        /* 将mpt page拷贝到mpt cache中 */
        memcpy(dst_addr, src_addr, PAGE_SIZE);
        //注意使用之后释放page索引！！！
        f2fs_put_page(mpt_page, 1);
    }
}

//读取LFU，将LFU访问计数数组保存到cache中
void f2fs_get_lfu(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    int i, start_blk, lfu_blk_num;
    struct page *lfu_page;
    void *dst_addr;
    void *src_addr;

    lfu_blk_num = (sbi->nsbi->nsb->main_segment_nums * sizeof(atomic_t) - 1) / PAGE_SIZE + 1;

    //TODO:此处要根据cp包中的记录来决定使用双份LFU中的哪一份
    start_blk = sbi->ckpt->ckpt_flags & CP_LFU_VER_FLAG ? nsbi->nsb->lfu_blkaddr1 : nsbi->nsb->lfu_blkaddr0;
    nvm_debug(NVM_DEBUG, "read from lfu_area: %d ", start_blk);

    for (i = 0; i < lfu_blk_num; ++i) {
        ///根据MPT版本位图得到当前有效的MPT page
        lfu_page = f2fs_get_meta_page(sbi, start_blk);
        src_addr = page_address(lfu_page);
        dst_addr = nsbi->lfu_count + (PAGE_SIZE * i) / sizeof(atomic_t);

        /* 将mpt page拷贝到mpt cache中 */
        memcpy(dst_addr, src_addr, PAGE_SIZE);
        //注意使用之后释放page索引！！！
        f2fs_put_page(lfu_page, 1);
        start_blk++;
    }

    // 输出调试信息
    // for(i=0; i<nsbi->nsb->main_segment_nums; i++){
    //     nvm_debug(1, "read from disk, segoff=%d, lfu_count=%d",i,atomic_read(&nsbi->lfu_count[i]));
    // }
}

/**
 * 1.初始化大根堆
 */
struct max_heap *init_max_heap(struct f2fs_sb_info *sbi, int k) {
    struct max_heap *mh = f2fs_kvzalloc(sbi, sizeof(struct max_heap), GFP_KERNEL);
    mh->nodes = f2fs_kvzalloc(sbi, k * sizeof(struct heap_node), GFP_KERNEL);
    mh->cur_num = 0;
    mh->k = k;
    return mh;
}

/**
 * 2.清除大根堆
 */
void clear_heap(struct max_heap *mh) {
    kvfree(mh->nodes);
    kvfree(mh);
    return;
}

/**
 * 3.判断大根堆是否为空
 */
bool is_empty_heap(struct max_heap *mh) {
    if (mh->cur_num == 0) {
        return true;
    }
    return false;
}

/**
 * 4.获取大根堆堆顶的元素值（堆中的最大元素）
 */
unsigned long get_max_heap_top(struct max_heap *mh) {
    if (is_empty_heap(mh)) {
        nvm_debug(NVM_DEBUG, "top-k max-heap is empty!!");
        return 0;
    }
    return mh->nodes[0].count;
}

/**
 * 判断某个SSD段，是否为上次checkpoint操作的当前段。
 */
bool is_last_ckpt_curseg(struct f2fs_checkpoint *ckpt, int ssd_segoff) {
    int node_h, node_w, node_c, data_h, data_w, data_c;
    node_h = le32_to_cpu(ckpt->cur_node_segno[CURSEG_HOT_NODE]);
    if (ssd_segoff == node_h)
        return true;
    node_w = le32_to_cpu(ckpt->cur_node_segno[CURSEG_WARM_NODE]);
    if (ssd_segoff == node_w)
        return true;
    node_c = le32_to_cpu(ckpt->cur_node_segno[CURSEG_COLD_NODE]);
    if (ssd_segoff == node_c)
        return true;

    data_h = le32_to_cpu(ckpt->cur_data_segno[CURSEG_HOT_DATA]);
    if (ssd_segoff == data_h)
        return true;
    data_w = le32_to_cpu(ckpt->cur_data_segno[CURSEG_WARM_DATA]);
    if (ssd_segoff == data_w)
        return true;
    data_c = le32_to_cpu(ckpt->cur_data_segno[CURSEG_COLD_DATA]);
    if (ssd_segoff == data_c)
        return true;
    return false;
}

/**
 * 4.获取访问计数列表的前k个最小元素，返回TOP K的大根堆
 */
void get_top_min_k(struct f2fs_sb_info *sbi, struct max_heap *mh, atomic_t *arr, int len) {
    int i;
    unsigned long count;
//    int ssd_segoff, secno;
//    struct f2fs_checkpoint *ckpt = F2FS_CKPT(sbi);

    //1.建立TOP k的大根堆
    // nvm_debug(1,">>>begin to build top-k max-heap");
    next:
    for (i = 0; i < len && mh->cur_num < mh->k; i++) {
        // if (!f2fs_test_bit(i, (char *) sbi->nsbi->ckpt_segment_map))
        //     continue;
        // ssd_segoff = get_mpt_value(sbi->nsbi, i);
        // if (is_last_ckpt_curseg(ckpt, ssd_segoff))
        //     continue;
        // secno = GET_SEC_FROM_SEG(sbi, ssd_segoff);
        // if (sec_usage_check(sbi, secno))//是当前在用的seg
        //     continue;

        count = atomic_read(&arr[i]);
        if (count < 0) {
            nvm_half_lfu_count(sbi);
            mh->cur_num = 0;
            goto next;
        }
        if (count > 0)
            insert_max_heap(mh, i, count);
    }
    // nvm_debug(1,"top-k max-heap is initialized，size of max-heap：%d",mh->cur_num);
    //2.遍历arr列表的剩余元素，获取top k个最小元素
    for (; i < len; i++) {
        // if (!f2fs_test_bit(i, (char *) sbi->nsbi->ckpt_segment_map))
        //     continue;
        // if (is_last_ckpt_curseg(ckpt, get_mpt_value(sbi->nsbi, i)))
        //     continue;
        // ssd_segoff = get_mpt_value(sbi->nsbi, i);
        // secno = GET_SEC_FROM_SEG(sbi, ssd_segoff);
        // if (sec_usage_check(sbi, secno))//跳过当前段
        //     continue;

        count = atomic_read(&arr[i]);
        if (count < 0) {
            nvm_half_lfu_count(sbi);
            mh->cur_num = 0;
            goto next;
        }
        // 剩余的元素中，如果有计数比大根堆堆顶元素小的，则加入挤占堆顶位置，并从堆顶开始自上而下调整大根堆
        if (count > 0 && count < get_max_heap_top(mh)) {
            mh->nodes[0].count = count;
            mh->nodes[0].seg_off = i;
            adjust_down(mh);
        }
    }
    // nvm_debug(1,">>>top-k max-heap is built");
    //3. 如果mh->cur_num < k 则说明arr列表中非0元素个数小于k个，即被访问的段的数量小于k个
    return;
}

/**
 * 4.向大根堆插入结点
 */
void insert_max_heap(struct max_heap *mh, int seg_off, int count) {
    int i, j;
    //1. 将新加入的元素添加到堆数组的末尾
    i = mh->cur_num;
//    mh->nodes[i].count = count;
//    mh->nodes[i].seg_off = seg_off;
    mh->cur_num++;

    // nvm_debug(1,"insert into max heap:%dth, segoff:%d, count:%d",mh->cur_num, mh->nodes[i].seg_off, mh->nodes[i].count);

    //2.自下而上，调整大根堆
    while (i > 0) {
        j = (i - 1) >> 1;// j指向i的双亲
        if (count <= mh->nodes[j].count)
            break;
        mh->nodes[i].count = mh->nodes[j].count;
        mh->nodes[i].seg_off = mh->nodes[j].seg_off;
        i = j;
    }
    mh->nodes[i].count = count;
    mh->nodes[i].seg_off = seg_off;
    return;
}

/**
 * 针对大根堆，自上而下进行调整
 */
void adjust_down(struct max_heap *mh) {
    int i = 0, j;
    int tmp_count = mh->nodes[0].count;
    int tmp_segoff = mh->nodes[0].seg_off;
    while (i < mh->cur_num) {
        j = (i + 1) * 2 - 1;// j是i的左孩子
        if (j < mh->cur_num - 1 && mh->nodes[j].count < mh->nodes[j + 1].count)
            j++;
        if (j >= mh->cur_num || tmp_count >= mh->nodes[j].count)
            break;
        mh->nodes[i].count = mh->nodes[j].count;
        mh->nodes[i].seg_off = mh->nodes[j].seg_off;
        i = j;
    }
    mh->nodes[i].count = tmp_count;
    mh->nodes[i].seg_off = tmp_segoff;
    return;
}


/**
* 打印大根堆
*/
void print_max_heap(struct max_heap *mh) {
    int i;
    if (is_empty_heap(mh)) {
        nvm_debug(1, "top-k max-heap is empty, nothing to output");
        return;
    }
    for (i = 0; i < mh->cur_num; i++) {
        nvm_debug(1, "top-k max heap, segoff:%d, count:%d", mh->nodes[i].seg_off, mh->nodes[i].count);
    }
}


void nvm_destory_nsbi_modules(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;

    /* 释放nsbi中的位图结构 */
    kvfree(nsbi->mpt);
    kvfree(nsbi->mpt_dirty_map);
    kvfree(nsbi->ckpt_segment_map);
    kvfree(nsbi->segment_map);
    kvfree(nsbi->mpt_ver_map);
    kvfree(nsbi->lfu_count);
    clear_heap(nsbi->mh);// 释放基于LFU访问计数的top-k max-heap
    __free_pages(nsbi->nvm_gc_start_page, sbi->log_blocks_per_seg);// 释放用于NVM-GC段迁移的连续物理内存页

    /*
    unsigned long r_re_data_bio_num;
    unsigned long r_re_data_page_num;
    unsigned long r_re_node_bio_num;
    unsigned long r_re_node_page_num;
    unsigned long r_data_bio_num;
    unsigned long r_data_page_num;
    unsigned long r_node_bio_num;
    unsigned long r_node_page_num;

    unsigned long w_re_data_bio_num;
    unsigned long w_re_data_page_num;
    unsigned long w_re_node_bio_num;
    unsigned long w_re_node_page_num;
    unsigned long w_data_bio_num;
    unsigned long w_data_page_num;
    unsigned long w_node_bio_num;
    unsigned long w_node_page_num;

    unsigned long r_re_meta_bio_num;
    unsigned long r_re_meta_page_num;
    unsigned long w_re_meta_bio_num;
    unsigned long w_re_meta_page_num;
    */
    nvm_debug(NVM_INFO, "r_re_data_bio_num:%d\n"
            "r_re_data_page_num:%d\n"
            "r_re_node_bio_num:%d\n"
            "r_re_node_page_num:%d\n"
            "r_data_bio_num:%d\n"
            "r_data_page_num:%d\n"
            "r_node_bio_num:%d\n"
            "r_node_page_num:%d\n"
            "w_re_data_bio_num:%d\n"
            "w_re_data_page_num:%d\n"
            "w_re_node_bio_num:%d\n"
            "w_re_node_page_num:%d\n"
            "w_data_bio_num:%d\n"
            "w_data_page_num:%d\n"
            "w_node_bio_num:%d\n"
            "w_node_page_num:%d\n"
            "r_re_meta_bio_num:%d\n"
            "r_re_meta_page_num:%d\n"
            "w_re_meta_bio_num:%d\n"
            "w_re_meta_page_num:%d",
              nsbi->r_re_data_bio_num,
              nsbi->r_re_data_page_num, nsbi->r_re_node_bio_num, nsbi->r_re_node_page_num, nsbi->r_data_bio_num,
              nsbi->r_data_page_num, nsbi->r_node_bio_num, nsbi->r_node_page_num,
              nsbi->w_re_data_bio_num, nsbi->w_re_data_page_num, nsbi->w_re_node_bio_num, nsbi->w_re_node_page_num,
              nsbi->w_data_bio_num, nsbi->w_data_page_num,
              nsbi->w_node_bio_num, nsbi->w_node_page_num, nsbi->r_re_meta_bio_num, nsbi->r_re_meta_page_num,
              nsbi->w_re_meta_bio_num, nsbi->w_re_meta_page_num);

    /* 释放nsb结构 */
    kfree(nsbi->nsb);

    /* 最后释放nsbi结构 */
    kfree(nsbi);

    sbi->nsbi = NULL;
}

//得到一个真实page的内容
void get_real_page(struct f2fs_sb_info *sbi, struct page *real_page, struct block_device *bdev, block_t blkno) {
    struct bio *read_bio;
    read_bio = f2fs_bio_alloc(sbi, 1, true);
//    f2fs_target_device(sbi, ssd_start_blkno, read_bio);//SSD转移到NVM
    bio_set_dev(read_bio, bdev);
    read_bio->bi_iter.bi_sector = SECTOR_FROM_BLOCK(blkno);
    read_bio->bi_private = NULL;

    if (bio_add_page(read_bio, real_page, PAGE_SIZE, 0) < PAGE_SIZE) {
        f2fs_bug_on(sbi, 1);
    }
    bio_set_op_attrs(read_bio, REQ_OP_READ, 0);
    //提交读操作
    submit_bio_wait(read_bio);
    bio_put(read_bio);
}
