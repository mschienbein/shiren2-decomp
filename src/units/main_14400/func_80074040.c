#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

extern s32 D_8013D8CC;

extern void *func_8006A8D8(char *name, u32 size);

void *func_80074040(void *name, s32 size)
{
    void *result;

    result = func_8006A8D8(name, size);
    if (result == 0) {
        D_8013D8CC = 0;
    }
    return result;
}
