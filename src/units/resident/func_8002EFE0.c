#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;

typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32];
    u8 label[32];
    s32 version;
    s32 dir_size;
    s32 inode_table;
    s32 minode_table;
    s32 dir_table;
    s32 inode_start_page;
    u8 banks;
    u8 activebank;
} OSPfs;

typedef union {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ipage;
} __OSInodeUnit;

typedef struct {
    __OSInodeUnit inode_page[128];
} __OSInode;

typedef struct {
    u32 game_code;
    u16 company_code;
    __OSInodeUnit start_page;
    u8 status;
    s8 reserved;
    u16 data_sum;
    u8 ext_name[4];
    u8 game_name[16];
} __OSDir;

s32 func_80026E94(OSPfs *pfs); /* __osCheckId */
s32 func_8002F640(OSPfs *pfs, u8 bank); /* __osPfsSelectBank */
s32 func_80027330(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer); /* __osContRamRead */
s32 func_80027520(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer, s32 force); /* __osContRamWrite */
s32 func_80026F4C(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank); /* __osPfsRWInode */
s32 func_8002E720(OSMesgQueue *queue, s32 channel); /* __osPfsGetStatus */

#define CHECK_IPAGE(p) (((p).ipage >= pfs->inode_start_page) && ((p).inode_t.bank < pfs->banks) && \
                        ((p).inode_t.page >= 0x01) && ((p).inode_t.page < 0x80))

static s32 __osPfsGetNextPage(OSPfs *pfs, u8 *bank, __OSInode *inode, __OSInodeUnit *page)
{
    s32 ret;

    if (page->inode_t.bank != *bank) {
        *bank = page->inode_t.bank;
        if ((ret = func_80026F4C(pfs, inode, 0, *bank)) != 0) {
            return ret;
        }
    }
    *page = inode->inode_page[page->inode_t.page];
    if (!CHECK_IPAGE(*page)) {
        if (page->ipage == 1) {
            return 5;
        }
        return 3;
    }
    return 0;
}

/* osPfsReadWriteFile */
s32 func_8002EFE0(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size_in_bytes, u8 *data_buffer)
{
    s32 ret;
    __OSDir dir;
    __OSInode inode;
    __OSInodeUnit cur_page;
    s32 cur_block;
    s32 siz_block;
    u8 *buffer;
    u8 bank;
    u16 blockno;

    if (file_no >= pfs->dir_size || file_no < 0) {
        return 5;
    }
    if (size_in_bytes <= 0 || (size_in_bytes % 32) != 0) {
        return 5;
    }
    if (offset < 0 || (offset % 32) != 0) {
        return 5;
    }
    if (!(pfs->status & 1)) {
        return 5;
    }
    if (func_80026E94(pfs) == 2) {
        return 2;
    }
    if (pfs->activebank != 0) {
        if ((ret = func_8002F640(pfs, 0)) != 0) {
            return ret;
        }
    }
    if ((ret = func_80027330(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir)) != 0) {
        return ret;
    }
    if (dir.company_code == 0 || dir.game_code == 0) {
        return 5;
    }
    if (!CHECK_IPAGE(dir.start_page)) {
        if (dir.start_page.ipage == 1) {
            return 5;
        }
        return 3;
    }
    if (flag == 0 && (dir.status & 2) == 0) {
        return 6;
    }
    bank = 0xFF;
    cur_block = offset / 32;
    cur_page = dir.start_page;
    while (cur_block >= 8) {
        if ((ret = __osPfsGetNextPage(pfs, &bank, &inode, &cur_page)) != 0) {
            return ret;
        }
        cur_block -= 8;
    }
    siz_block = size_in_bytes / 32;
    buffer = data_buffer;
    while (siz_block > 0) {
        if (cur_block == 8) {
            if ((ret = __osPfsGetNextPage(pfs, &bank, &inode, &cur_page)) != 0) {
                return ret;
            }
            cur_block = 0;
        }
        if (pfs->activebank != cur_page.inode_t.bank) {
            if ((ret = func_8002F640(pfs, cur_page.inode_t.bank)) != 0) {
                return ret;
            }
        }
        blockno = cur_page.inode_t.page * 8 + cur_block;
        if (flag == 0) {
            ret = func_80027330(pfs->queue, pfs->channel, blockno, buffer);
        } else {
            ret = func_80027520(pfs->queue, pfs->channel, blockno, buffer, 0);
        }
        if (ret != 0) {
            return ret;
        }
        buffer += 32;
        cur_block++;
        siz_block--;
    }
    if (flag == 1 && !(dir.status & 2)) {
        dir.status |= 2;
        if (pfs->activebank != 0) {
            if ((ret = func_8002F640(pfs, 0)) != 0) {
                return ret;
            }
        }
        if ((ret = func_80027520(pfs->queue, pfs->channel, (u16)(pfs->dir_table + file_no), (u8 *)&dir, 0)) != 0) {
            return ret;
        }
    }
    ret = func_8002E720(pfs->queue, pfs->channel);
    return ret;
}
