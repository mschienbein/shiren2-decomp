#include "common.h"

typedef struct {
    char pad0[8];
    void *vtable8;
} Obj;

extern unsigned char D_80160428[];
extern void *func_80115690(void *obj, s32 kind);

Obj *func_80126280(Obj *obj) {
    func_80115690(obj, 0xE6);
    obj->vtable8 = D_80160428;
    return obj;
}
