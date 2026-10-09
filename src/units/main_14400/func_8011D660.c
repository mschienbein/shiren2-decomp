#include "common.h"

typedef struct {
    char pad0[0x8];
    void *vtable;
} Obj;

extern char D_8015ED38[];
extern Obj *func_80112470(Obj *obj, s32 id);

Obj *func_8011D660(Obj *obj) {
    func_80112470(obj, 0x7A);
    obj->vtable = D_8015ED38;
    return obj;
}
