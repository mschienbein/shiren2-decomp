#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015E640[];
/* Returns the input object through this TU's partial view. */
extern Obj *func_80112D20(Obj *obj, s32 kind);

Obj *func_8011AB60(Obj *obj) {
    func_80112D20(obj, 0x1E);
    obj->vtable = D_8015E640;
    return obj;
}
