#include "common.h"

typedef struct {
    char pad0[8];
    void *vtable8;
    s32 fieldC;
} Obj;

extern unsigned char D_8015FCA0[];
extern void *func_8010E020(void *object, s32 kind);

Obj *func_80123A80(Obj *obj) {
    func_8010E020(obj, 0xCD);
    obj->vtable8 = D_8015FCA0;
    obj->fieldC = 1000000;
    return obj;
}
