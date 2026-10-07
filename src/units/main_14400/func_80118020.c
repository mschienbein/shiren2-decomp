#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable_8;
} Obj;

extern Obj *func_80116D50(Obj *obj, s32 kind);
extern char D_8015DE40[];

Obj *func_80118020(Obj *obj) {
    func_80116D50(obj, 9);
    obj->vtable_8 = D_8015DE40;
    return obj;
}
