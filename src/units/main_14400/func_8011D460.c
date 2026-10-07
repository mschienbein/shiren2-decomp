#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015ECF0[];
extern Obj *func_80112470(Obj *obj, s32 kind);

Obj *func_8011D460(Obj *obj) {
    func_80112470(obj, 0x79);
    obj->vtable = D_8015ECF0;
    return obj;
}
