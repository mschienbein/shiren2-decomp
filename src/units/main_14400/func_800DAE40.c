#include "common.h"

typedef struct {
    unsigned char pad0[4];
    void *vtable;
} Obj;

extern unsigned char D_801583B8[];
extern void *func_800DA8A0(void *obj, s32 kind, void *src);

Obj *func_800DAE40(Obj *obj, void *src) {
    func_800DA8A0(obj, 0xC, src);
    obj->vtable = D_801583B8;
    return obj;
}
