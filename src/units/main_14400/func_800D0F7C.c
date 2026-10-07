#include "common.h"

typedef unsigned char u8;
typedef struct { void *field_0; s32 field_4; s32 field_8; } Obj;
u8 func_800AFFD0(void *src_table, void *obj, void *dst_table);
void func_800D0F7C(Obj *o, void *b, void **c) {
    func_800AFFD0(o->field_0, b, *c);
    o->field_8 = 1;
}
