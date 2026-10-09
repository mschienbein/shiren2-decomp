#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xC];
    u8 flags_0C;
} Obj80116B64;

s32 func_80116B64(Obj80116B64 *obj)
{
    return (obj->flags_0C >> 1) & 1;
}
