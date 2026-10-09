#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[8];
    const void *field_8;
} Obj;

extern const unsigned char D_8015DC88[88];
Obj *func_80116D50(Obj *, s32);

Obj *func_80117820(Obj *obj) {
    func_80116D50(obj, 0x4);
    obj->field_8 = D_8015DC88;
    return obj;
}
