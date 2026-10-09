#include "common.h"

typedef struct {
    char pad0[0x8];
    void *vtable;
} Obj;

extern char D_8015E690[];
extern Obj *func_80112D20(Obj *obj, s32 id);

Obj *func_8011AD10(Obj *obj) {
    func_80112D20(obj, 0x1F);
    obj->vtable = D_8015E690;
    return obj;
}
