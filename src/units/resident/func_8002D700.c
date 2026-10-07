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
extern s32 func_8002E610(OSPfs *pfs, s32 *bytes_not_used);

s32 func_8002DA0C(OSPfs *pfs, __OSInode *inode, s32 file_size_in_pages, s32 *first_page, u8 bank, s32 *decleared, s32 *last_page);

s32 func_8002D700(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 file_size_in_bytes, s32 *file_no)
{
    s32 start_page;
    s32 decleared;
    s32 last_page;
    s32 old_last_page = 0;
    s32 ret = 0;
    s32 file_size_in_pages;
    __OSInode inode;
    __OSInode backup_inode;
    __OSDir dir;
    u8 bank;
    u8 old_bank = 0;
    s32 firsttime = 0;
    s32 bytes;
    __OSInodeUnit fpage;

    if (company_code == 0 || game_code == 0) {
        return 5;
    }
    file_size_in_pages = (file_size_in_bytes + 255) / 256;

    if ((ret = func_8002F480(pfs, company_code, game_code, game_name, ext_name, file_no)) != 0 && ret != 5) {
        return ret;
    }
    if (*file_no != -1) {
        return 9;
    }
    ret = func_8002E610(pfs, &bytes);
    if (file_size_in_bytes > bytes) {
        return 7;
    }
    if (file_size_in_pages == 0) {
        return 5;
    }
    if ((ret = func_8002F480(pfs, 0, 0, 0, 0, file_no)) != 0 && ret != 5) {
        return ret;
    }
    if (*file_no == -1) {
        return 8;
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        if ((ret = func_80026F4C(pfs, &inode, 0, bank)) != 0) {
            return ret;
        }
        if ((ret = func_8002DA0C(pfs, &inode, file_size_in_pages, &start_page, bank, &decleared, &last_page)) != 0) {
            return ret;
        }
        if (start_page != -1) {
            if (firsttime == 0) {
                fpage.inode_t.page = start_page;
                fpage.inode_t.bank = bank;
            } else {
                backup_inode.inode_page[old_last_page].inode_t.bank = bank;
                backup_inode.inode_page[old_last_page].inode_t.page = start_page;
                if ((ret = func_80026F4C(pfs, &backup_inode, 1, old_bank)) != 0) {
                    return ret;
                }
            }
            if (file_size_in_pages > decleared) {
                func_800262C0(&inode, &backup_inode, sizeof(__OSInode));
                old_last_page = last_page;
                old_bank = bank;
                file_size_in_pages -= decleared;
                firsttime++;
            } else {
                file_size_in_pages = 0;
                if ((ret = func_80026F4C(pfs, &inode, 1, bank)) != 0) {
                    return ret;
                }
                break;
            }
        }
    }

    if (file_size_in_pages > 0 || start_page == -1) {
        return 3;
    }

    dir.start_page = fpage;
    dir.company_code = company_code;
    dir.game_code = game_code;
    dir.data_sum = 0;
    func_800262C0(game_name, dir.game_name, 16);
    func_800262C0(ext_name, dir.ext_name, 4);
    ret = func_80027520(pfs->queue, pfs->channel, (u16)(pfs->dir_table + *file_no), (u8 *)&dir, 0);
    return ret;
}

s32 func_8002DA0C(OSPfs *pfs, __OSInode *inode, s32 file_size_in_pages, s32 *first_page, u8 bank, s32 *decleared, s32 *last_page)
{
    s32 j;
    s32 spage;
    s32 old_page;
    s32 ret = 0;
    s32 offset = (bank > 0) ? 1 : pfs->inode_start_page;

    for (j = offset; j < 128; j++) {
        if (inode->inode_page[j].ipage == 3) {
            break;
        }
    }
    if (j == 128) {
        *first_page = -1;
        return ret;
    }

    spage = j;
    *decleared = 1;
    old_page = j;
    j++;
    while (file_size_in_pages > *decleared && j < 128) {
        if (inode->inode_page[j].ipage == 3) {
            inode->inode_page[old_page].inode_t.bank = bank;
            inode->inode_page[old_page].inode_t.page = j;
            old_page = j;
            (*decleared)++;
        }
        j++;
    }

    *first_page = spage;
    if (j == 128 && file_size_in_pages > *decleared) {
        *last_page = old_page;
    } else {
        inode->inode_page[old_page].ipage = 1;
        *last_page = 0;
    }
    return ret;
}
