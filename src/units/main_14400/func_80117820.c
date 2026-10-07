#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[8];
    void *field_8;
} Obj;

extern s32 D_8015DC88;
Obj *func_80116D50(Obj *, s32);

Obj *func_80117820(Obj *obj) {
    func_80116D50(obj, 0x4);
    obj->field_8 = &D_8015DC88;
    return obj;
}
