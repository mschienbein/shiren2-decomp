#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[8];
    void *field_8;
} Obj;

extern s32 D_801600B8;
Obj *func_80115690(Obj *, s32);

Obj *func_80124EE0(Obj *obj) {
    func_80115690(obj, 0xDB);
    obj->field_8 = &D_801600B8;
    return obj;
}
