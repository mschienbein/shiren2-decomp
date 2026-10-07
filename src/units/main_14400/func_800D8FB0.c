#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 data[0xCC]; } Buf800D8FB0;
extern s32 D_80148250;
extern Buf800D8FB0 D_801C9CF0[];

Buf800D8FB0 *func_800D8FB0(u32 size) {
    D_80148250 ^= 1;
    return &D_801C9CF0[D_80148250];
}
