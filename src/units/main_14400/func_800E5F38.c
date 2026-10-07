#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 a;
    u32 b;
} Pair;

typedef struct {
    u8 pad0[0x52];
    u8 field_52;
    u8 field_53;
    u8 pad54[0x10];
    Pair field_64;
} Obj800E5F38;

void func_800E5F38(Obj800E5F38 *obj, Pair *src)
{
    obj->field_64 = *src;
    obj->field_52 = 2;
    obj->field_53 = 1;
}
