#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015FB20[];
extern Obj *func_80114060(Obj *obj, s32 kind);

Obj *func_80123230(Obj *obj) {
    func_80114060(obj, 0xB2);
    obj->vtable = D_8015FB20;
    return obj;
}
