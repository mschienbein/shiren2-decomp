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

s32 func_80027520(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer, s32 force);

s32 func_8002F640(OSPfs *pfs, u8 bank)
{
    u8 temp[32];
    s32 i;
    s32 ret;

    for (i = 0; i < 32; i++) {
        temp[i] = bank;
    }
    ret = func_80027520(pfs->queue, pfs->channel, 0x400, temp, 0);
    if (ret == 0) {
        pfs->activebank = bank;
    }
    return ret;
}
