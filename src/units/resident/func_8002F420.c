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

s32 func_80026CC8(OSPfs *pfs);

s32 func_8002F420(OSPfs *pfs)
{
    s32 ret;

    if (pfs->status & 5) {
        ret = func_80026CC8(pfs);
        if (ret == 0) {
            pfs->status &= ~4;
        }
    } else {
        ret = 5;
    }
    return ret;
}
