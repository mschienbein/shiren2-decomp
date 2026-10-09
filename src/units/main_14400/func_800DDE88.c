#include "common.h"

typedef struct {
    unsigned char pad0[4];
    void *vtable;
} Obj;

extern unsigned char D_80158928[];
extern Obj *func_800DDAD0(Obj *obj, s32 arg1);
extern void func_800DDC0C(Obj *obj, unsigned char *data, s32 length);

Obj *func_800DDE88(Obj *obj, unsigned char *data) {
    s32 length;

    func_800DDAD0(obj, 0x28);
    obj->vtable = D_80158928;
    length = *data++;
    func_800DDC0C(obj, data, length - 1);
    return obj;
}
