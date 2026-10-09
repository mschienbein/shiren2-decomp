#include "common.h"

typedef unsigned char u8;

/* Partial view: only the flag byte at 0x0C is read. */
typedef struct Obj80116B94 {
    u8 pad_00[0xC];
    u8 flags_0C;
} Obj80116B94;

s32 func_80116B94(Obj80116B94 *obj)
{
    return obj->flags_0C & 1;
}
