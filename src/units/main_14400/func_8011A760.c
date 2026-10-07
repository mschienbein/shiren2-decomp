#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable_8;
} Obj;

/* Returns the input object through this TU's partial view. */
extern Obj *func_80112D20(Obj *obj, s32 kind);
extern char D_8015E5A0[];

Obj *func_8011A760(Obj *obj) {
    func_80112D20(obj, 0x1C);
    obj->vtable_8 = D_8015E5A0;
    return obj;
}
