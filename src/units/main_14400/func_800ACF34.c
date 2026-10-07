#include "common.h"

typedef unsigned char u8;

typedef struct { u8 field_0; u8 id; u8 flags; } Obj800ACF34;

void func_800AD3F4(u8 id);

void func_800ACF34(Obj800ACF34 *obj) {
    obj->flags |= 2;
    func_800AD3F4(obj->id);
}
