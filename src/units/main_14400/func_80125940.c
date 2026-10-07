#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable_8;
} Obj;

extern Obj *func_80115690(Obj *obj, s32 kind);
extern char D_801602E8[];

Obj *func_80125940(Obj *obj) {
    func_80115690(obj, 0xE2);
    obj->vtable_8 = D_801602E8;
    return obj;
}
