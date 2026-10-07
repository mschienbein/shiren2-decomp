#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_8015EAC0[];
/* Returns the input object through this TU's partial view. */
Obj *func_80112D20(Obj *obj, s32 kind);

Obj *func_8011C560(Obj *obj)
{
    func_80112D20(obj, 43);
    obj->vtable = D_8015EAC0;
    return obj;
}
