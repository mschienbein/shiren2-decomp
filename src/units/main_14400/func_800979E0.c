#include "common.h"

typedef struct { char pad0[0x4C]; void *field_4C; } Obj;
extern Obj D_80140470;
extern char D_80152260[];
Obj *func_800953C0(Obj *obj);

void func_800979E0(void) {
    Obj *obj = &D_80140470;

    func_800953C0(obj);
    obj->field_4C = D_80152260;
}
