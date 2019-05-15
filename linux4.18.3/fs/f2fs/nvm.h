//
// Created by ypf on 18-11-29.
//
#ifndef __F2FS_NVM_H__
#define __F2FS_NVM_H__

#include <linux/fs.h>
#include <linux/f2fs_fs.h>
#include "f2fs.h"

//标志nsb版本：块0或块1
#define CP_NSB_VER_FLAG    0x00001000
//标志LFU版本：第一份LFU有效，还是第二份LFU有效
#define CP_LFU_VER_FLAG    0x00010000
//nvm MAIN区域相对于SSD MAIN区域段偏移
#define MAIN_OFFSET 2

/*NVM标志位*/
//记录nsb是否变脏并回写
#define NVM_NSB_DIRTY 0x01
//是否第一次挂载，读META转移到NVM设备
#define NVM_FIRST_MOUNR 0x02
//第一个超级块块号
#define NVM_SUPER_BLOCK 0
//NVM最低空闲段百分比，空闲段低于该值回收段
#define NVM_LIMIT_FREE 10
//有效块数目低于时比例则直接使用原GC回收段
#define SEG_VALID_BLOCKS 30
//NVM进行垃圾回收时，一次回收的段数目
#define NVM_LFU_FREE 50

/*调试部分*/
//断言-括号内为需要满足条件
#define nvm_assert(assert)    BUG_ON(!(assert))

enum nvm_debug_type {
    NVM_ERR,//0
    NVM_INFO,//1
    NVM_DEBUG//2
};
//调试控制,小于该等级才会打印
#define __NVM_DEBUG__ 3
#if __NVM_DEBUG__ > 0

void __nvm_debug(int level, const char *func,
                 unsigned int line, const char *fmt, ...);

#define nvm_debug(n, fmt, a...) \
    __nvm_debug((n), __func__, __LINE__, (fmt), ##a)
#else
#define nvm_debug(n, fmt, a...)
#endif

struct f2fs_sb_info;
enum page_type;

///nvm超级块，对应磁盘结构
///该结构部分信息需要保证一致性
struct nvm_super_block {
    __u8 uuid[16];//128-bit uuid，与所要加速的SSD设备UUID相同
    __le32 mpt_blkaddr;//映射表区域起始块地址
    __le32 main_blkaddr;//NVM中MAIN区域起始块地址
    __le32 lfu_blkaddr0;//第一个lfu NVM段访问计数数组的块地址
    __le32 lfu_blkaddr1;//第二个lfu NVM段访问计数数组的块地址
    unsigned int main_first_segno;//nvm MAIN区域首个segment的segment号
    __le32 main_segment_nums;//NVM中MAIN区域segment总数，该值也是segment位图（segment_map）的位数
    __le32 main_segment_free_nums;//NVM中MAIN区域空闲segment总数
    unsigned int mpt_ver_map_bits;//MPT版本位图（mpt_ver_map）的位数，理论上=mpt_dirty_map_bits
    unsigned int mpt_entries;//MPT表的项数，理论=SSD segs+NVM segs
    char map[1];//硬盘中超级块从该位置开始，存储了以下两者map信息,这个结构仅仅用作定位
//    unsigned long * mpt_ver_map;  //mpt版本位图：记录每个有效mpt块在双份seg中的哪一个
//    unsigned long * segment_map;  //main区域segment有效性位图
};

///与nvm相关的内存结构
struct nvm_sb_info {
    struct nvm_super_block *nsb;//关联nvm超级块
    struct block_device *nbdev;//指向NVM设备信息的指针。初始化时根据ndev_path建立block_device结构并关联到nbdev
    //记录nsb是否变脏并回写，ckpt_flag判断这个字段来决定是否更换nsb版本号
    __u8 nvm_flag;//NVM标志位
    ///GC相关
    struct f2fs_gc_kthread *gc_thread;    /*nvm GC thread */
    struct mutex nvmgc_mutex;            /* mutex for nvmGC */

    ///MPT表中下标是segment相对main区域的偏移量【SSD表项在NVM表项后面，是NVM表项+在SSD中偏移量】，
    /// 值记录的是映射的segment在所在设备中，相对于MAIN区域首部的segment偏移量
    unsigned int *mpt;//MPT区域在内存中的Cache,挂载时设置，checkpoint时同步到硬盘

    //MPT表脏页位图：每一个位代表一个MPT page是否为脏，总位数也就是MPT page的数目。映射项被修改后将该项所在页标记为脏，用于回写
    unsigned long *mpt_dirty_map;
    unsigned int mpt_dirty_map_bits;//mpt_dirty_map的位数
    //每次对mpt的修改都会修改mpt_dirty_map，所以mpt_lock用于锁定mpt_dirty_map和mpt的映射表项修改
    spinlock_t mpt_lock;//mpt_dirty_map、mpt lock

    char *mpt_ver_map;//MPT版本位图：用于双备份机制，记录每个有效MPT块在双份segment中的哪一个。每一位代表MPT区域一个page
    spinlock_t mpt_ver_map_lock;//mpt_ver_map lock

    unsigned long *segment_map;//MAIN区域segment有效性位图【只针对MAIN区域，也就是说第一位是MAIN区域第一个segment号】
    /**
     * 用于保存前一次cp的segment位图，避免重用前一次cp释放的segment导致崩溃一致性问题
     * ckpt_segment_map只保留内存结构，初始化时候设置和segment_map一样；
     * 分配segment时候需要判断两个位图都是0的才能使用；
     * 两次cp之间分配或者释放都只更改segment_map，在每次cp结束时刻ckpt_segment_map保持与segment_map一样
     */
    unsigned long *ckpt_segment_map;
    spinlock_t segment_map_lock;//segment_map、ckpt_segment_map lock

    unsigned int cur_alloc_nvm_segoff;// NVM段分配时的当前段偏移量

    ///TODO: 添加段访问计数器，用于NVM容量紧张时，以LFU的方式进行垃圾回收
    atomic_t *lfu_count;// 调专门的函数进行加减
    struct max_heap *mh;// 基于LFU访问计数建立的大根堆，保存了待回收的NVM受害者段

    struct page *nvm_gc_start_page;// 用于NVM-GC段迁移
    struct mutex nvm_gc_curseg_lock;// 锁定当前回收的段，避免回收期间的写操作

    // 以下统计信息针对缓存IO
    unsigned long r_re_data_page_num;
    unsigned long r_re_data_bio_num;
    unsigned long r_data_page_num;
    unsigned long r_data_bio_num;
    unsigned long r_re_node_page_num;
    unsigned long r_re_node_bio_num;
    unsigned long r_node_page_num;
    unsigned long r_node_bio_num;
    unsigned long r_re_meta_page_num;
    unsigned long r_re_meta_bio_num;

    unsigned long w_re_data_page_num;
    unsigned long w_re_data_bio_num;
    unsigned long w_data_page_num;
    unsigned long w_data_bio_num;
    unsigned long w_re_node_page_num;
    unsigned long w_re_node_bio_num;
    unsigned long w_node_page_num;
    unsigned long w_node_bio_num;
    unsigned long w_re_meta_page_num;
    unsigned long w_re_meta_bio_num;

    // 以下统计信息针对直接IO
    unsigned long r_re_page_num;
    unsigned long r_not_re_page_num;
    unsigned long w_re_page_num;
    unsigned long w_not_re_page_num;

    unsigned long r_re_bio_num;
    unsigned long r_not_re_bio_num;
    unsigned long w_re_bio_num;
    unsigned long w_not_re_bio_num;

    // 以下统计信息针对GC
    unsigned long whole_seg_gc_num;// 段迁移的数量
    unsigned long block_gc_seg_num;// 块迁移涉及到的段的数量
    unsigned long block_gc_num;// 块迁移的总数


};

//MPT区域每个项重定向标志：位于映射项低位端
enum redirect_flag {
    ///当读写都不需要重定向，说明没有映射
            RED_READ,//读是否重定向
    RED_WRITE,//写是否重定向
    NR_FLAGS
};

/* 下面的堆结构，用于NVM GC，选取访问计数最少的前k个段 */
struct heap_node {
    unsigned long count;    /* 该段的访问计数 */
    unsigned int seg_off;    /* NVM MAIN区域的段号 */
};

struct max_heap {
    struct heap_node *nodes;/* 数组，保存所有的大根堆结点 */
    unsigned int cur_num;    /* 大根堆当前的结点数目 */
    unsigned int k;            /* 表示TOP K 的大根堆 */
};

static inline void set_mpt_entry(struct nvm_sb_info *nsbi, unsigned int index, unsigned int offset, int redirect_flag) {
    nsbi->mpt[index] = ((offset << NR_FLAGS) | redirect_flag);
}

static inline int get_mpt_entry(struct nvm_sb_info *nsbi, unsigned int index) {
    return nsbi->mpt[index];
}

static inline int get_redirect_flag(struct nvm_sb_info *nsbi, int index) {
    int mask = (0x1 << NR_FLAGS) - 0x1;
    return nsbi->mpt[index] & mask;
}

static inline void set_redirect_flag(struct nvm_sb_info *nsbi, int index, int redirect_flag) {
    nsbi->mpt[index] = ((nsbi->mpt[index] >> NR_FLAGS) << NR_FLAGS) | redirect_flag;
}

static inline int get_mpt_value(struct nvm_sb_info *nsbi, int index) {
    return nsbi->mpt[index] >> NR_FLAGS;
}

/**
 * 内联函数部分，切换nsb版本
 */
static inline void nvm_switch_nsb_version(struct f2fs_checkpoint *ckpt) {
    ckpt->ckpt_flags ^= CP_NSB_VER_FLAG;
}

// 切换lfu版本
static inline void nvm_switch_lfu_version(struct f2fs_checkpoint *ckpt) {
    ckpt->ckpt_flags ^= CP_LFU_VER_FLAG;// __le32 ckpt_flags;
}

//当nvm空闲段数目低于NVM_LIMIT_FREE，开始回收段
static inline bool has_not_enough_free_nvm_secs(struct nvm_sb_info *nsbi) {
    //根据百分比直接判断
    return nsbi->nsb->main_segment_free_nums < nsbi->nsb->main_segment_nums * NVM_LIMIT_FREE / 100;
}

static inline void reset_max_heap(struct max_heap *mh) {
    mh->cur_num = 0;
}

/**
 * nvm.c函数声明
 */

//将cache中mpt刷回mapping page
void nvm_flush_mpt_pages(struct f2fs_sb_info *sbi, int flush_all);

//将cache中nsb刷回mapping page
void nvm_flush_nsb_pages(struct f2fs_sb_info *sbi, pgoff_t block_addr);

//将cache中lfu刷回mapping page
void nvm_flush_lfu_pages(struct f2fs_sb_info *sbi, int flag);

//获取下一个要写的mpt区域有效块号
struct page *get_next_mpt_page(struct f2fs_sb_info *sbi, int mpt_pgno);

//submit_bio之前，进行bio重定向
struct bio *nvm_redirect_bio(struct f2fs_sb_info *sbi, struct bio *bio, enum page_type type);
struct bio *nvm_redirect_bio_test(struct f2fs_sb_info *sbi, struct bio *bio, enum page_type type);

//执行直接IO时，对buffer_head结构进行重定向
int nvm_redirect_dio(struct f2fs_sb_info *sbi, struct buffer_head *bh);
int nvm_redirect_dio_test(struct f2fs_sb_info *sbi, struct buffer_head *bh);

//从nvm分配块并建立映射关系
int nvm_allocate_segment(struct f2fs_sb_info *sbi, unsigned int ssd_segno);

//在每次cp之后，拷贝前一次cp的有效位图到ckpt_segment_map
void nvm_set_ckpt_segment_map(struct f2fs_sb_info *sbi);

/* 读取SSD全部META区域 */
int read_meta_page_from_SSD(struct f2fs_sb_info *sbi);

//写回NVM META区域
int flush_meta_page_to_NVM(struct f2fs_sb_info *sbi);

//读NVM超级块
int read_nvm_super_block(struct f2fs_sb_info *sbi, int *recovery);

/* 检查NVM超级块*/
int sanity_check_nvm_super(struct f2fs_sb_info *sbi,
                           struct nvm_super_block *nsb);

//依据nsb设置nsbi的相关字段
unsigned int init_nvm_sb_info(struct f2fs_sb_info *sbi, struct nvm_super_block *nsb);

//获取当前使用的有效mpt page（函数主要选定双份seg中有效的那个）
struct page *get_current_mpt_page(struct f2fs_sb_info *sbi, int mpt_pgoff);

//读取MPT映射表，将mpt表保存到cache中
void f2fs_get_mpt(struct f2fs_sb_info *sbi);

//读取LFU，将LFU访问计数数组保存到cache中
void f2fs_get_lfu(struct f2fs_sb_info *sbi);

/**
 * gc_nvm.c函数声明
 */
int f2fs_nvm_gc(struct f2fs_sb_info *sbi, bool sync,
                bool background, unsigned int segno);

//为NVM相关代码调用__submit_bio开放接口
void nvm_submit_bio(struct f2fs_sb_info *sbi, struct bio *bio, enum page_type type);

void unset_mapping(struct f2fs_sb_info *sbi, unsigned int segno);

void nvm_destory_nsbi_modules(struct f2fs_sb_info *sbi);

int f2fs_start_nvm_gc_thread(struct f2fs_sb_info *sbi);

/*start: 以下几个函数用于建立top-k的大根堆 */
struct max_heap *init_max_heap(struct f2fs_sb_info *sbi, int k);

void clear_heap(struct max_heap *mh);

bool is_empty_heap(struct max_heap *mh);

bool is_last_ckpt_curseg(struct f2fs_checkpoint *ckpt, int ssd_segoff);

void nvm_half_lfu_count(struct f2fs_sb_info *sbi);

unsigned long get_max_heap_top(struct max_heap *mh);

void get_top_min_k(struct f2fs_sb_info *sbi, struct max_heap *mh, atomic_t *arr, int len);

void insert_max_heap(struct max_heap *mh, int seg_off, int count);

void adjust_down(struct max_heap *mh);

void print_max_heap(struct max_heap *mh);

#endif /* __F2FS_NVM_H__ */
