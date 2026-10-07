#include "common.h"

typedef struct { unsigned char pad[8]; void *vtable; } Object;
extern char D_8015FF20[];
Object *func_80115690(Object *obj, s32 kind);
Object *func_80124980(Object *obj) {
    func_80115690(obj, 0xD7);
    obj->vtable = D_8015FF20;
    return obj;
}
