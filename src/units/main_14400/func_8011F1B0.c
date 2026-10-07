#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[8];
    void *field_8;
} Obj;

extern s32 D_8015F1C0;
/* Returns the input object through this TU's partial view. */
Obj *func_80111530(Obj *, s32);

Obj *func_8011F1B0(Obj *obj) {
    func_80111530(obj, 0x97);
    obj->field_8 = &D_8015F1C0;
    return obj;
}
