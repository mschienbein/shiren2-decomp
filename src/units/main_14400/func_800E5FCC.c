#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pair_800E5FCC;

typedef struct { u8 value; } Dir;

typedef struct {
    u8 pad0[0x52];
    u8 field_52;
    u8 field_53;
    u8 pad54[0x10];
    Pair_800E5FCC pair_64;
} Obj_800E5FCC;

extern void *func_800A25D8(void *out, void *from, Dir dir, s32 scale);

void func_800E5FCC(Obj_800E5FCC *obj, Dir *b, s16 c) {
    Dir copy = *b;
    Pair_800E5FCC tmp;

    func_800A25D8(&tmp, obj, copy, c);
    obj->pair_64 = tmp;
    obj->field_52 = 2;
    obj->field_53 = 1;
}
