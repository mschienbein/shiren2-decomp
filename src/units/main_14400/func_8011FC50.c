#include "common.h"

typedef struct {
    char pad0[0x8];
    void *vtable;
} Obj;

extern char D_8015F4A8[];
extern Obj *func_80111530(Obj *obj, s32 id);

Obj *func_8011FC50(Obj *obj) {
    func_80111530(obj, 0xA0);
    obj->vtable = D_8015F4A8;
    return obj;
}
