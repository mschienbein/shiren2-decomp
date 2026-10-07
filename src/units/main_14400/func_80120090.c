#include "common.h"

typedef struct { unsigned char pad[8]; void *vtable; } Object;
extern char D_8015F668[];
void func_8010E290(Object *obj, s32 kind);
Object *func_80120090(Object *obj) {
    func_8010E290(obj, 0xA4);
    obj->vtable = D_8015F668;
    return obj;
}
