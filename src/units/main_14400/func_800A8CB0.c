#include "common.h"

typedef unsigned char u8;

/* Cell ids use the first 29 of thirty 0xE4-byte records; the last is the fallback. */
typedef struct {
    unsigned char data[0xE4];
} Record;

extern unsigned char D_801C35E0[0x10C];
extern Record D_801C36EC[30];

void *func_800A8CB0(s32 cell)
{
    u8 index;

    if ((u8)cell == 0) {
        return D_801C35E0;
    }
    index = cell - 1;
    return index < 29 ? &D_801C36EC[index] : 0;
}
