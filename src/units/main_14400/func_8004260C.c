#include "common.h"

typedef unsigned char u8;
/* 800C9E00 returns D_801C9BD8, whose complete backing object is 0xC8 bytes. */
typedef struct { u8 bytes[0xC8]; } State;
extern void *func_800C9E00(void);

u8 func_8004260C(s32 row, s32 column)
{
    u8 *bytes = ((State *)func_800C9E00())->bytes;
    row *= 4;
    column += row;
    bytes += column;
    return bytes[0x43];
}
