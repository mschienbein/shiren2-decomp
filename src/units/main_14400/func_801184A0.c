#include "common.h"

typedef struct {
    char pad0[0x8];
    void *vtable;
} Obj;

extern char D_8015DF48[];
extern Obj *func_80116D50(Obj *obj, s32 id);

Obj *func_801184A0(Obj *obj) {
    func_80116D50(obj, 0xC);
    obj->vtable = D_8015DF48;
    return obj;
}
