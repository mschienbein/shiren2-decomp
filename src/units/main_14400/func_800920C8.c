#include "common.h"

/* +0x4 holds a descriptor pointer (descriptor+0 row count, descriptor+4 array of
 * 0x14-byte rows) read by the base operations func_80091A1C/func_80091B30;
 * the descriptor stays opaque in this constructor. */
typedef struct {
    s32 field_0;
    void *descriptor;
    s32 pad8;
    void *volatile vtbl;
    s32 pad10[5];
    s32 field_24;
    s32 pad28[3];
    s32 field_34;
    s32 pad38[3];
    void *field_44;
    s32 pad48[3];
    void *field_54;
} Obj;
extern s32 D_80151350[];
extern s32 D_801513B0[];
extern s32 D_80151480[];

Obj *func_800920C8(Obj *o, void *descriptor) {
    o->vtbl = D_80151350;
    o->vtbl = D_801513B0;
    o->field_24 = -1;
    o->field_34 = -1;
    o->descriptor = descriptor;
    o->field_44 = D_80151480;
    o->field_54 = D_80151480;
    return o;
}
