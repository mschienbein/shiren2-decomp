#include "common.h"

typedef struct { unsigned char pad[8]; void *vtable; } Object;
extern char D_8015F010[];
/* Returns the input object through this TU's partial view. */
Object *func_80111530(Object *obj, s32 kind);
Object *func_8011E7C0(Object *obj) {
    func_80111530(obj, 0x93);
    obj->vtable = D_8015F010;
    return obj;
}
