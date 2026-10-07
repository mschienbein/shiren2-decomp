#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable_8;
} Obj;

extern Obj *func_80114060(Obj *obj, s32 kind);
extern char D_8015FA30[];

Obj *func_80122D70(Obj *obj) {
    func_80114060(obj, 0xAF);
    obj->vtable_8 = D_8015FA30;
    return obj;
}
