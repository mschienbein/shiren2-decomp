#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct S80121348 S80121348;

extern void *func_800AC5B4(s32 size, s32 alternate);
extern S80121348 *func_80121348(S80121348 *p);

S80121348 *func_80121320(void)
{
    return func_80121348(func_800AC5B4(0x2C, 0));
}
