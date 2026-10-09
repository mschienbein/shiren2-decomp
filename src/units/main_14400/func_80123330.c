#include "common.h"

typedef struct {
    char pad0[8];
    void *vtable8;
} Obj;

extern unsigned char D_8015FB70[];
extern void *func_8010E5C0(void *obj, s32 arg1);

Obj *func_80123330(Obj *obj) {
    func_8010E5C0(obj, 0xC8);
    obj->vtable8 = D_8015FB70;
    return obj;
}
