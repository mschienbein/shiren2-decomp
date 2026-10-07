#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xB2];
    u16 field_B2;
    u16 field_B4;
} Obj80129470;

u8 *func_80129470(Obj80129470 *obj, u8 *p)
{
    u32 value;

    value = *p++ << 8;
    value |= *p++;
    obj->field_B2 = value;
    obj->field_B4 = 0;
    return p;
}
