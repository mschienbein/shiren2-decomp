#include "common.h"

typedef struct { unsigned char pad[8]; void *vtable; } Object;
extern char D_8015E158[];
Object *func_80116D50(Object *obj, s32 kind);
Object *func_80118C80(Object *obj) {
    func_80116D50(obj, 0x12);
    obj->vtable = D_8015E158;
    return obj;
}
