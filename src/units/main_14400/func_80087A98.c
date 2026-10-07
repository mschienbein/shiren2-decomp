#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Partial view of the record; only the touched fields are known. */
typedef struct {
    char pad0[0x4];
    u16 field_4;
    char pad6[0x8];
    u16 field_E;
    char pad10[0x4];
    s32 index;
} Obj;

extern u8 D_801C3395[];

void func_80087A98(Obj *obj)
{
    D_801C3395[obj->index] = 0;
    obj->field_E = 1;
    obj->field_4 = 4;
}
