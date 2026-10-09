#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xC];
    u8 flags_0C;
} Obj80128B84;

s32 func_80128B84(Obj80128B84 *obj)
{
    return obj->flags_0C & 1;
}
