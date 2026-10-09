#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 field_0;
    void *field_4;
} Obj800DDDE0;

extern u8 D_80158928[];

Obj800DDDE0 *func_800DDAD0(Obj800DDDE0 *obj, s32 size);

Obj800DDDE0 *func_800DDDE0(Obj800DDDE0 *obj) {
    func_800DDAD0(obj, 0x28);
    obj->field_4 = D_80158928;
    return obj;
}
