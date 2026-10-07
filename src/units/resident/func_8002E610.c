#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 status;
    void *queue;
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

extern s32 func_80026E94(OSPfs *pfs);
extern s32 func_80026F4C(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);

s32 func_8002E610(OSPfs *pfs, s32 *bytes_not_used)
{
    s32 j;
    s32 pages = 0;
    __OSInode inode;
    s32 ret = 0;
    u8 bank;
    s32 offset;

    if ((pfs->status & 1) == 0) {
        return 5;
    }
    if ((ret = func_80026E94(pfs)) != 0) {
        return ret;
    }
    for (bank = 0; bank < pfs->banks; bank++) {
        if ((ret = func_80026F4C(pfs, &inode, 0, bank)) != 0) {
            return ret;
        }
        offset = (bank > 0) ? 1 : pfs->inode_start_page;
        for (j = offset; j < 128; j++) {
            if (inode.inode_page[j].ipage == 3) {
                pages++;
            }
        }
    }
    *bytes_not_used = pages << 8;
    return 0;
}
