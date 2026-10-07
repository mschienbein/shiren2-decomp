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

typedef struct {
    __OSInode inode;
    u8 bank;
    u8 map[256];
} __OSInodeCache;

s32 func_8002DF90(OSPfs *pfs, __OSInodeCache *cache)
{
    s32 i;
    s32 n;
    s32 offset;
    u8 bank;
    __OSInodeUnit tpage;
    __OSInode tmp_inode;
    s32 ret;

    for (i = 0; i < 256; i++) {
        cache->map[i] = 0;
    }
    cache->bank = -1;

    for (bank = 0; bank < pfs->banks; bank++) {
        offset = (bank > 0) ? 1 : pfs->inode_start_page;
        ret = func_80026F4C(pfs, &tmp_inode, 0, bank);
        if (ret != 0 && ret != 3) {
            return ret;
        }
        for (i = offset; i < 128; i++) {
            tpage = tmp_inode.inode_page[i];
            if (tpage.ipage >= pfs->inode_start_page && tpage.inode_t.bank != bank) {
                n = ((tpage.inode_t.page & 0x7F) / 4) + 32 * (tpage.inode_t.bank % 8);
                cache->map[n] |= (1 << (bank % 8));
            }
        }
    }
    return 0;
}

s32 func_8002E0E8(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache)
{
    s32 j;
    s32 n;
    s32 hit = 0;
    u8 bank;
    s32 offset;
    s32 ret = 0;

    n = (fpage.inode_t.page / 4) + 32 * (fpage.inode_t.bank % 8);

    for (bank = 0; bank < pfs->banks; bank++) {
        offset = (bank > 0) ? 1 : pfs->inode_start_page;
        if (bank == fpage.inode_t.bank || (cache->map[n] & (1 << (bank % 8)))) {
            if (bank != cache->bank) {
                ret = func_80026F4C(pfs, &cache->inode, 0, bank);
                if (ret != 0 && ret != 3) {
                    return ret;
                }
                cache->bank = bank;
            }
            for (j = offset; (hit < 2) && (j < 128); j++) {
                if (cache->inode.inode_page[j].ipage == fpage.ipage) {
                    hit++;
                }
            }
            if (1 < hit) {
                return 2;
            }
        }
    }
    return hit;
}
