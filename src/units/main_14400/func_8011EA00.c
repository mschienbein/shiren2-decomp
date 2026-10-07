#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_8015F0F0[];
/* Returns the input object through this TU's partial view. */
Obj *func_80111530(Obj *obj, s32 kind);

Obj *func_8011EA00(Obj *obj)
{
    func_80111530(obj, 149);
    obj->vtable = D_8015F0F0;
    return obj;
}
