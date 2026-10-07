#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015F440[];
/* Returns the input object through this TU's partial view. */
extern Obj *func_80111530(Obj *obj, s32 kind);

Obj *func_8011FB50(Obj *obj) {
    func_80111530(obj, 0x9E);
    obj->vtable = D_8015F440;
    return obj;
}
