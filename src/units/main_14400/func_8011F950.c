#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable_8;
} Obj;

/* Returns the input object through this TU's partial view. */
extern Obj *func_80111530(Obj *obj, s32 kind);
extern char D_8015F370[];

Obj *func_8011F950(Obj *obj) {
    func_80111530(obj, 0x9C);
    obj->vtable_8 = D_8015F370;
    return obj;
}
