#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x72];
    u8 flags_72;
} Obj800E2610;

void func_800E2610(Obj800E2610 *obj)
{
    obj->flags_72 &= ~4;
}
