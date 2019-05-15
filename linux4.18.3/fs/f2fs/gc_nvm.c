/*
 * fs/f2fs/gc.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/backing-dev.h>
#include <linux/init.h>
#include <linux/f2fs_fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/freezer.h>


#include "f2fs.h"
#include "node.h"
#include "segment.h"
#include "gc.h"
#include "nvm.h"

/**
 * 在原有的gc函数中，segno与segoff同义
 * 沿用此定义，本文件中所有segno均与segoff同义
 */

static int nvm_gc_thread_func(void *data) {
    struct f2fs_sb_info *sbi = data;
    struct f2fs_gc_kthread *gc_th = sbi->nsbi->gc_thread;
    wait_queue_head_t *wq = &sbi->nsbi->gc_thread->gc_wait_queue_head;
    unsigned int wait_ms;

    wait_ms = gc_th->min_sleep_time;

    set_freezable();
    do {
        wait_event_interruptible_timeout(*wq,
                                         kthread_should_stop() || freezing(current) ||
                                         gc_th->gc_wake,
                                         msecs_to_jiffies(wait_ms));

        /* give it a try one time */
        if (gc_th->gc_wake)
            gc_th->gc_wake = 0;

        if (try_to_freeze())
            continue;
        if (kthread_should_stop())
            break;

        if (sbi->sb->s_writers.frozen >= SB_FREEZE_WRITE) {/* 超级块被冻结时，增加GC的睡眠时间 */
            increase_sleep_time(gc_th, &wait_ms);
            continue;
        }

#ifdef CONFIG_F2FS_FAULT_INJECTION
        if (time_to_inject(sbi, FAULT_CHECKPOINT)) {
            f2fs_show_injection_info(FAULT_CHECKPOINT);
            f2fs_stop_checkpoint(sbi, false);
        }
#endif

        if (!sb_start_write_trylock(sbi->sb))
            continue; 

        if (!mutex_trylock(&sbi->nsbi->nvmgc_mutex))
            goto next;

        if (!has_not_enough_free_nvm_secs(sbi->nsbi)) {
            goto next;
        }
        //TODO:nvm gc
        f2fs_nvm_gc(sbi, test_opt(sbi, FORCE_FG_GC), true,
                    NULL_SEGNO);//test_opt与mount有关，(sbi->mount_opt).opt &0x00004000

        /* balancing f2fs's metadata periodically */
        f2fs_balance_fs_bg(sbi);
        next:
        mutex_unlock(&sbi->nsbi->nvmgc_mutex);
        sb_end_write(sbi->sb);
    } while (!kthread_should_stop());
    return 0;
}

int f2fs_start_nvm_gc_thread(struct f2fs_sb_info *sbi) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    struct f2fs_gc_kthread *gc_th;
    dev_t dev = nsbi->nbdev->bd_dev;
    int err = 0;

    gc_th = f2fs_kmalloc(sbi, sizeof(struct f2fs_gc_kthread), GFP_KERNEL);
    if (!gc_th) {
        err = -ENOMEM;
        goto out;
    }

    gc_th->urgent_sleep_time = DEF_GC_THREAD_URGENT_SLEEP_TIME;
    gc_th->min_sleep_time = DEF_GC_THREAD_MIN_SLEEP_TIME;
    gc_th->max_sleep_time = DEF_GC_THREAD_MAX_SLEEP_TIME;
    gc_th->no_gc_sleep_time = DEF_GC_THREAD_NOGC_SLEEP_TIME;

    gc_th->gc_wake = 0;

    nsbi->gc_thread = gc_th;
    init_waitqueue_head(&nsbi->gc_thread->gc_wait_queue_head);
    nsbi->gc_thread->f2fs_gc_task = kthread_run(nvm_gc_thread_func, sbi, "f2fs_gc-%u:%u", MAJOR(dev), MINOR(dev));

    if (IS_ERR(gc_th->f2fs_gc_task)) {
        err = PTR_ERR(gc_th->f2fs_gc_task);
        kfree(gc_th);
        sbi->nsbi->gc_thread = NULL;
    }
    out:
    return err;
}
/*
static int get_nvm_victim(struct f2fs_sb_info *sbi, unsigned int *result)//用result返回NVM的受害者的segno
{
    struct nvm_sb_info *nsbi = sbi->nsbi;
    unsigned int secno;
    unsigned int start_segno = 0;//NVM和SSD的segno都是从main区域的第一个段开始，同segoff
//    unsigned int last_segno = sbi->nsbi->nsb->main_segment_nums;
//    unsigned int nsearched = sbi->nsbi->nsb->main_segment_nums;//每次查找的范围，后期可能会改这个范围
    unsigned int segno = start_segno;//段号查找段空闲位图得到
    unsigned int ssd_segno;
    unsigned long changetime;

    if (*result != NULL_SEGNO && get_redirect_flag(sbi->nsbi, *result)) {
        ssd_segno = get_mpt_value(sbi->nsbi, *result);
        if (!sec_usage_check(sbi, GET_SEC_FROM_SEG(sbi, ssd_segno)))//不是当前在用的seg
            goto out;
    }

    changetime = ((unsigned long) (~0));

    //因为此处只读取位图状态，不需要加锁
    segno = find_next_bit(nsbi->segment_map, nsbi->nsb->main_segment_nums, segno);
    while (segno != nsbi->nsb->main_segment_nums) {

        ssd_segno = get_mpt_value(sbi->nsbi, segno);
        secno = GET_SEC_FROM_SEG(sbi, ssd_segno);
        if (sec_usage_check(sbi, secno))//是当前在用的seg
            continue;

        if (changetime > get_seg_entry(sbi, ssd_segno)->mtime) {//找其中的修改最早的段
            changetime = get_seg_entry(sbi, ssd_segno)->mtime;
            *result = segno;
        }

        segno = find_next_bit(nsbi->segment_map, nsbi->nsb->main_segment_nums, ++segno);
    }

    out:
    if (*result == NULL_SEGNO)
        nvm_debug(NVM_DEBUG, "get nvm victim(segno is %d)", *result);
    else
        nvm_debug(NVM_DEBUG, "get nvm victim(result is %d)", *result);
    return 1;//不存在找不到的情况，此函数一定会成功
}
*/
//nvm_segoff是段在NVM中相对于main区域的偏移
static void gc_nvm_segment(struct f2fs_sb_info *sbi, unsigned int nvm_segoff) {
    block_t ssd_start_blkno, nvm_start_blkno;
    int flag = 0;//对每个段分两个bio读写
    struct page *read_start_page, *write_start_page;
    struct bio *read_bio;
    struct bio *write_bio;
    enum page_type type = IS_DATASEG(get_seg_entry(sbi, get_mpt_value(sbi->nsbi, nvm_segoff))->type) ?
                          DATA : NODE;
    unsigned int ssd_segoff;
//    unsigned int entries_per_blk = F2FS_BLKSIZE / sizeof(unsigned int);
    int i;//循环控制

    nvm_debug(NVM_DEBUG, ">>>>>>>>>start gc_nvm_segment");
    if (!get_redirect_flag(sbi->nsbi, nvm_segoff)) {//此NVM段没有建立映射，无需回收
        return;
    }


    // start_page = alloc_pages(GFP_KERNEL, sbi->log_blocks_per_seg);
    read_start_page = sbi->nsbi->nvm_gc_start_page;
    write_start_page = sbi->nsbi->nvm_gc_start_page;
    nvm_assert(read_start_page);

//    start_page=page_address(start_page);
//    nvm_debug(NVM_DEBUG, "start_page addr:%p", start_page);
    ssd_segoff = get_mpt_value(sbi->nsbi, nvm_segoff);
    ///修改重定向标志：在读重定向，写不重定向标志下，所有读操作都从nvm进行，并且保证所有文件写操作都不会发生【不回收当前段】
    ///对于ssr模式，需要保证不使用正处于读重定向，写不重定向标志下的段
    set_redirect_flag(sbi->nsbi, nvm_segoff, 1);//读重定向，写不重定向
    set_redirect_flag(sbi->nsbi, ssd_segoff + sbi->nsbi->nsb->main_segment_nums, 1);//读重定向，写不重定向

    //设置nvm和ssd起始块地址
    nvm_start_blkno = nvm_segoff * sbi->blocks_per_seg + sbi->nsbi->nsb->main_blkaddr;

    ssd_start_blkno = START_BLOCK(sbi, ssd_segoff);//ssd中对应段的首地址
    //数据段转移
    once:
    flag++;
    //从nvm读数据
    read_bio = f2fs_bio_alloc(sbi, BIO_MAX_PAGES, true);
//    f2fs_target_device(sbi, ssd_start_blkno, read_bio);//SSD转移到NVM
    bio_set_dev(read_bio, sbi->nsbi->nbdev);
    read_bio->bi_iter.bi_sector = SECTOR_FROM_BLOCK(nvm_start_blkno);
    read_bio->bi_private = NULL;

    nvm_debug(NVM_DEBUG, "bio,nvm blkno:%d,ssd blkno:%d", nvm_start_blkno, ssd_start_blkno);

    for (i = 0; i < sbi->blocks_per_seg / 2; i++) {
        if (bio_add_page(read_bio, read_start_page, PAGE_SIZE, 0) < PAGE_SIZE)
            f2fs_bug_on(sbi, 1);
        read_start_page++;
    }
    bio_set_op_attrs(read_bio, REQ_OP_READ, 0);
    //提交读操作
    //nvm_debug(NVM_DEBUG,"nvm_submit_bio");
//    nvm_submit_bio(sbi, read_bio, type);
    submit_bio_wait(read_bio);
    bio_put(read_bio);

    //写数据到ssd
    write_bio = f2fs_bio_alloc(sbi, BIO_MAX_PAGES, true);
    f2fs_target_device(sbi, ssd_start_blkno, write_bio);//这一次不会转移到NVM
    write_bio->bi_write_hint = f2fs_io_type_to_rw_hint(sbi, type, HOT);
    for (i = 0; i < sbi->blocks_per_seg / 2; i++) {
//        lock_page(now_page);
//        set_page_dirty(now_page);
        if (bio_add_page(write_bio, write_start_page, PAGE_SIZE, 0) < PAGE_SIZE)
            f2fs_bug_on(sbi, 1);
        write_start_page++;
    }
    bio_set_op_attrs(write_bio, REQ_OP_WRITE, REQ_SYNC);//REQ_SYNC或者REQ_BACKGROUND，目前都用REQ_SYNC
    //nvm_submit_bio(sbi, write_bio, type);

    submit_bio_wait(write_bio);
//    nvm_submit_bio(sbi, write_bio, type);
    bio_put(write_bio);

    //更新起始位置为段的后半部分
    nvm_start_blkno += sbi->blocks_per_seg / 2;
    ssd_start_blkno += sbi->blocks_per_seg / 2;
    if (flag != 2)goto once;

    // __free_pages(tmp_page, sbi->log_blocks_per_seg);//释放内存页
    //取消映射关系，设置脏页信息。
    nvm_debug(NVM_DEBUG, "nvm gc,unset_mapping,nvm segoff:%d,ssd segoff:%d,free segs:%d", nvm_segoff, ssd_segoff,
              sbi->nsbi->nsb->main_segment_free_nums);
    unset_mapping(sbi, ssd_segoff);
}

static int do_nvm_garbage_collect(struct f2fs_sb_info *sbi, unsigned int start_segno) {
    struct blk_plug plug;
    unsigned int segno = start_segno;//nvm段号
    unsigned int end_segno = start_segno + sbi->segs_per_sec;
    int seg_freed = 0;

    blk_start_plug(&plug);

    for (segno = start_segno; segno < end_segno; segno++) {
        gc_nvm_segment(sbi, segno);
        seg_freed++;
    }

    blk_finish_plug(&plug);

    return seg_freed;
}

int f2fs_nvm_gc(struct f2fs_sb_info *sbi, bool sync, bool background, unsigned int segno) {
    int gc_type = sync ? FG_GC : BG_GC;
    int sec_freed = 0, seg_freed = 0, total_freed = 0, secno, ssd_segno;
    int i, ret = 0;
    int nvm_main_seg_num = sbi->nsbi->nsb->main_segment_nums;
    struct max_heap *mh = sbi->nsbi->mh;
    struct cp_control cpc;
    unsigned int valid_blocks;

//    nvm_debug(NVM_DEBUG,"nvm gc start");

    gc_more:
    if (gc_type == BG_GC && has_not_enough_free_nvm_secs(sbi->nsbi)) {
/*
		 * For example, if there are many prefree_segments below given
		 * threshold, we can make them free by checkpoint. Then, we
		 * secure free segments which doesn't need fggc any more.
		 */
        if (prefree_segments(sbi)) {
            cpc.reason = __get_cp_reason(sbi);
            ret = f2fs_write_checkpoint(sbi, &cpc);
            if (ret)
                goto stop;
        }
        if (has_not_enough_free_nvm_secs(sbi->nsbi))
            gc_type = FG_GC;
    }

    /* f2fs_balance_fs doesn't need to do BG_GC in critical path. */
    if (gc_type == BG_GC && !background) {
        ret = -EINVAL;
        goto stop;
    }

    ///TODO: 选取NVM受害者段，通过LFU 大根堆选择访问技术最少的前k个段
    /* start */
    // struct max_heap *mh = init_max_heap(sbi, NVM_LFU_FREE);
    reset_max_heap(mh);// 将当前大根堆内当前节点数置0
    get_top_min_k(sbi, mh, sbi->nsbi->lfu_count, nvm_main_seg_num);
    if (mh->cur_num == 0) {
        goto stop;
    }

    //3. 遍历大根堆，针对每个选中的NVM段进行GC操作
    for (i = 0; i < mh->cur_num; i++) {
        segno = mh->nodes[i].seg_off;
        ssd_segno = get_mpt_value(sbi->nsbi, segno);
        nvm_debug(NVM_DEBUG, "%dth element in top-k max-heap, nvm_segoff=%d, ssd_segoff=%d, lfu_count=%d", i, segno,
                  ssd_segno,
                  mh->nodes[i].count);

        secno = GET_SEC_FROM_SEG(sbi, ssd_segno);
        if (sec_usage_check(sbi, secno))//是当前在用的seg
            continue;
        //对于有效块少的段，直接使用F2FS原有的GC机制回收
        valid_blocks = get_valid_blocks(sbi, ssd_segno, false);
        if (valid_blocks == 0)continue;//有效块为0的不做GC，等待ckpt回收
        if (valid_blocks < sbi->blocks_per_seg * SEG_VALID_BLOCKS / 100) {
            struct gc_inode_list gc_list = {
                    .ilist = LIST_HEAD_INIT(gc_list.ilist),
                    .iroot = RADIX_TREE_INIT(gc_list.iroot, GFP_NOFS),
            };
            nvm_debug(NVM_DEBUG, "less valid blocks=(%d),nvm_segoff=(%d),ssd_segoff=(%d)", valid_blocks, segno,
                      ssd_segno);
            mutex_lock(&sbi->gc_mutex);
            seg_freed = do_garbage_collect(sbi, ssd_segno, &gc_list, gc_type);
//            nvm_assert(seg_freed != 0);
            mutex_unlock(&sbi->gc_mutex);
        } else {
            mutex_lock(&sbi->nsbi->nvm_gc_curseg_lock);
            seg_freed = do_nvm_garbage_collect(sbi, segno);
            mutex_unlock(&sbi->nsbi->nvm_gc_curseg_lock);
        }
        // if (gc_type == FG_GC && seg_freed == sbi->segs_per_sec)
        //     sec_freed++;
        if (gc_type == FG_GC && seg_freed == sbi->segs_per_sec) {
            sec_freed++;
            nvm_debug(NVM_DEBUG, "nvm_segment_number:%d has been GC", segno);
        }

        total_freed += seg_freed;
    }

    if (!sync) {
        if (has_not_enough_free_nvm_secs(sbi->nsbi)) {
            segno = NULL_SEGNO;
            goto gc_more;
        }
    }
    stop:
    if (sync)
        ret = sec_freed ? 0 : -EAGAIN;
//    nvm_debug(NVM_DEBUG,"nvm gc end");
    return ret;
}

/**
 * 如果该SSD段存在映射，则取消映射
 * 这里的segno同义于ssd的segoff
 * @segno:ssd中要回收的段相对于ssd main区域偏移
 */
void unset_mapping(struct f2fs_sb_info *sbi, unsigned int segno) {
    struct nvm_sb_info *nsbi = sbi->nsbi;
    unsigned int ssd_offset = segno + nsbi->nsb->main_segment_nums;
    unsigned int nvm_offset;
    //mpt区域每个块能保存多少项
    unsigned int entries_per_blk = F2FS_BLKSIZE / sizeof(unsigned int);
    if (get_redirect_flag(nsbi, ssd_offset)) {
//        nvm_debug(NVM_DEBUG, "unset mapping,ssd blkoff:%d", segno);
        spin_lock(&nsbi->mpt_lock);
        spin_lock(&nsbi->segment_map_lock);
        /*取消映射关系，设置脏页信息*/
        nvm_offset = get_mpt_value(nsbi, ssd_offset);
        atomic_set(&nsbi->lfu_count[nvm_offset], 0x0);
        nsbi->mpt[ssd_offset] = 0;
        nsbi->mpt[nvm_offset] = 0;
        set_bit(ssd_offset / entries_per_blk, nsbi->mpt_dirty_map);
        set_bit(nvm_offset / entries_per_blk, nsbi->mpt_dirty_map);
        //修改segment位图，增加空闲segment数目
        clear_bit(nvm_offset, nsbi->segment_map);
        nsbi->nsb->main_segment_free_nums++;
        spin_unlock(&nsbi->segment_map_lock);
        spin_unlock(&nsbi->mpt_lock);
    }
}
