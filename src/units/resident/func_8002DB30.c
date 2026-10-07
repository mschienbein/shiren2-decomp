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
extern s32 func_80026CC8(OSPfs *pfs);

typedef struct {
    __OSInode inode;
    u8 bank;
    u8 map[256];
} __OSInodeCache;

extern s32 func_8002DF90(OSPfs *pfs, __OSInodeCache *cache);
extern s32 func_8002E0E8(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache);

s32 func_8002DB30(OSPfs *pfs)
{
    s32 j;
    s32 ret;
    __OSInodeUnit next_page;
    __OSInode checked_inode;
    __OSInode tmp_inode;
    __OSDir tmp_dir;
    __OSInodeUnit file_next_node[16];
    __OSInodeCache cache;
    s32 fixed = 0;
    u8 bank;
    u8 oldbank = 254;
    s32 cc;
    s32 cl;
    s32 offset;

    ret = func_80026E94(pfs);
    if (ret == 2) {
        ret = func_80026CC8(pfs);
    }
    if (ret != 0) {
        return ret;
    }
    if ((ret = func_8002DF90(pfs, &cache)) != 0) {
        return ret;
    }

    for (j = 0; j < pfs->dir_size; j++) {
        if ((ret = func_80027330(pfs->queue, pfs->channel, (u16)(pfs->dir_table + j), (u8 *)&tmp_dir)) != 0) {
            return ret;
        }
        if (tmp_dir.company_code != 0 || tmp_dir.game_code != 0) {
            if (tmp_dir.company_code == 0 || tmp_dir.game_code == 0) {
                cc = -1;
            } else {
                next_page = tmp_dir.start_page;
                cl = cc = 0;
                bank = 0xFF;
                while (next_page.ipage >= pfs->inode_start_page && next_page.inode_t.bank < pfs->banks
                       && next_page.inode_t.page > 0 && next_page.inode_t.page < 128) {
                    if (bank != next_page.inode_t.bank) {
                        bank = next_page.inode_t.bank;
                        if (oldbank != bank) {
                            ret = func_80026F4C(pfs, &tmp_inode, 0, bank);
                            oldbank = bank;
                        }
                        if (ret != 0 && ret != 3) {
                            return ret;
                        }
                    }
                    if ((cc = func_8002E0E8(pfs, next_page, &cache) - cl) != 0) {
                        break;
                    }
                    cl = 1;
                    next_page = tmp_inode.inode_page[next_page.inode_t.page];
                }
            }
            if (cc != 0 || next_page.ipage != 1) {
                func_800265E0(&tmp_dir, sizeof(__OSDir));
                if (pfs->activebank != 0) {
                    if ((ret = func_8002F640(pfs, 0)) != 0) {
                        return ret;
                    }
                }
                if ((ret = func_80027520(pfs->queue, pfs->channel, (u16)(pfs->dir_table + j), (u8 *)&tmp_dir, 0)) != 0) {
                    return ret;
                }
                fixed++;
            }
        }
    }

    for (j = 0; j < pfs->dir_size; j++) {
        if ((ret = func_80027330(pfs->queue, pfs->channel, (u16)(pfs->dir_table + j), (u8 *)&tmp_dir)) != 0) {
            return ret;
        }
        if (tmp_dir.company_code != 0 && tmp_dir.game_code != 0
            && tmp_dir.start_page.ipage >= (u16)pfs->inode_start_page) {
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        } else {
            file_next_node[j].ipage = 0;
        }
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        ret = func_80026F4C(pfs, &tmp_inode, 0, bank);
        if (ret != 0 && ret != 3) {
            return ret;
        }
        offset = (bank > 0) ? 1 : pfs->inode_start_page;
        for (j = 0; j < offset; j++) {
            checked_inode.inode_page[j].ipage = tmp_inode.inode_page[j].ipage;
        }
        for (; j < 128; j++) {
            checked_inode.inode_page[j].ipage = 3;
        }
        for (j = 0; j < pfs->dir_size; j++) {
            while (file_next_node[j].inode_t.bank == bank
                   && file_next_node[j].ipage >= (u16)pfs->inode_start_page) {
                u8 pp = file_next_node[j].inode_t.page;
                file_next_node[j] = checked_inode.inode_page[pp] = tmp_inode.inode_page[pp];
            }
        }
        if ((ret = func_80026F4C(pfs, &checked_inode, 1, bank)) != 0) {
            return ret;
        }
    }

    if (fixed) {
        pfs->status |= 2;
    } else {
        pfs->status &= ~2;
    }
    return 0;
}
