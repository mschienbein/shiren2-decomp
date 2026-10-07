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
s32 func_8002F640(OSPfs *pfs, u8 bank); /* __osPfsSelectBank */
s32 func_80027330(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer); /* __osContRamRead */
s32 func_8002E720(OSMesgQueue *queue, s32 channel); /* __osPfsGetStatus */

s32 func_8002EEE0(OSPfs *pfs, s32 *max_files, s32 *files_used)
{
    s32 j;
    s32 ret;
    __OSDir dir;
    s32 files = 0;

    if (!(pfs->status & 1)) {
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
    for (j = 0; j < pfs->dir_size; j++) {
        if ((ret = func_80027330(pfs->queue, pfs->channel, (u16)(pfs->dir_table + j), (u8 *)&dir)) != 0) {
            return ret;
        }
        if (dir.company_code != 0 && dir.game_code != 0) {
            files++;
        }
    }
    *files_used = files;
    *max_files = pfs->dir_size;
    ret = func_8002E720(pfs->queue, pfs->channel);
    return ret;
}
