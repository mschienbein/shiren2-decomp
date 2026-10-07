#include "common.h"

typedef struct {
    s32 field_0;
    s32 field_4;
    void *vtable_8;
} Obj;

extern Obj *func_80112470(Obj *obj, s32 kind);
extern char D_80148FD0[];

Obj *func_8011D3A0(Obj *obj) {
    func_80112470(obj, 0x77);
    obj->vtable_8 = D_80148FD0;
    return obj;
}
