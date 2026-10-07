#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[8];
    void *field_8;
} Obj;

extern s32 D_8015F7A8;
Obj *func_80114060(Obj *, s32);

Obj *func_80120910(Obj *obj) {
    func_80114060(obj, 0xA8);
    obj->field_8 = &D_8015F7A8;
    return obj;
}
