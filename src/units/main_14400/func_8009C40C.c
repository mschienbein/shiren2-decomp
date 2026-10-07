#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x4C];
    void *vtable;
} Obj;

extern u8 D_801528D0[];
extern Obj *func_800953C0(Obj *obj);

Obj *func_8009C40C(Obj *obj) {
    func_800953C0(obj);
    obj->vtable = D_801528D0;
    return obj;
}
