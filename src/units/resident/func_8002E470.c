#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;

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

typedef struct {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;

extern s32 func_80026E94(OSPfs *pfs);
extern s32 func_80026F4C(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
extern s32 func_8002F640(OSPfs *pfs, u8 bank);
extern s32 func_80027330(void *queue, s32 channel, u16 address, u8 *buffer);
extern s32 func_80027520(void *queue, s32 channel, u16 address, u8 *buffer, s32 force);
extern void *func_800262C0(const void *src, void *dst, s32 length);
extern void func_800265E0(void *dst, s32 length);
extern s32 func_8002E720(void *queue, s32 channel);
extern s32 func_8002F480(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no);


s32 func_8002E470(OSPfs *pfs, s32 file_no, OSPfsState *state)
{
    s32 ret;
    s32 pages;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit next_page;
    u8 bank;

    if (file_no >= pfs->dir_size || file_no < 0) {
        return 5;
    }
    if ((pfs->status & 1) == 0) {
        return 5;
    }
    if ((ret = func_80026E94(pfs)) != 0) {
        return ret;
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

    pages = 0;
    next_page = dir.start_page;
    bank = 255;
    while (1) {
        if (next_page.ipage < pfs->inode_start_page) {
            break;
        }
        if (next_page.inode_t.bank != bank) {
            bank = next_page.inode_t.bank;
            if ((ret = func_80026F4C(pfs, &inode, 0, bank)) != 0) {
                return ret;
            }
        }
        pages++;
        next_page = inode.inode_page[next_page.inode_t.page];
    }

    if (next_page.ipage != 1) {
        return 3;
    }

    state->file_size = pages << 8;
    state->company_code = dir.company_code;
    state->game_code = dir.game_code;
    func_800262C0(&dir.game_name, state->game_name, 16);
    func_800262C0(&dir.ext_name, state->ext_name, 4);
    return func_8002E720(pfs->queue, pfs->channel);
}
