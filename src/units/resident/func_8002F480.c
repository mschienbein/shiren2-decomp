#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
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

typedef struct {
    u32 game_code;
    u16 company_code;
    u16 start_page;
    u8 status;
    s8 reserved;
    u16 data_sum;
    u8 ext_name[4];
    u8 game_name[16];
} __OSDir;

s32 func_80026E94(OSPfs *pfs); /* __osCheckId */
s32 func_80027330(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer); /* __osContRamRead */
s32 func_8002E720(OSMesgQueue *queue, s32 channel); /* __osPfsGetStatus */

/* osPfsFindFile */
s32 func_8002F480(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 *file_no)
{
    s32 j;
    s32 i;
    __OSDir dir;
    s32 ret = 0;
    s32 fail;

    if (!(pfs->status & 1)) {
        return 5;
    }
    if ((ret = func_80026E94(pfs)) != 0) {
        return ret;
    }
    for (j = 0; j < pfs->dir_size; j++) {
        if ((ret = func_80027330(pfs->queue, pfs->channel, (u16)(pfs->dir_table + j), (u8 *)&dir)) != 0) {
            return ret;
        }
        if ((ret = func_8002E720(pfs->queue, pfs->channel)) != 0) {
            return ret;
        }
        if (dir.company_code == company_code && dir.game_code == game_code) {
            fail = 0;
            if (game_name != 0) {
                for (i = 0; i < 16; i++) {
                    if (dir.game_name[i] != game_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if (ext_name != 0 && fail == 0) {
                for (i = 0; i < 4; i++) {
                    if (dir.ext_name[i] != ext_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if (fail == 0) {
                *file_no = j;
                return ret;
            }
        }
    }
    *file_no = -1;
    return 5;
}
