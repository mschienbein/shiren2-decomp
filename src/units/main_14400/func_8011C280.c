#include "common.h"

typedef struct { unsigned char pad[8]; void *vtable; } Object;
extern char D_8015EA20[];
/* Returns the input object through this TU's partial view. */
Object *func_80112D20(Object *obj, s32 kind);
Object *func_8011C280(Object *obj) {
    func_80112D20(obj, 0x29);
    obj->vtable = D_8015EA20;
    return obj;
}
