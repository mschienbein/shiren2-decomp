#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x60]; short delta_60, index_62; void (*reset_64)(void *); u8 pad_68[0x28]; short delta_90, index_92; s32 (*action_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *vtable_24; } Obj;
typedef Obj S;
typedef struct { u8 pad_0[0x13]; u8 field_13; u8 pad_14[3]; u8 field_17; } Values;
extern s32 func_800E1CD4(S *, s32);
extern void func_800E43EC(Obj *, unsigned short, u8);
extern s32 func_800A533C(u8 *a);
/* Slots 0x64/0x94 include func_800ECC74 and func_800EA38C. */
s32 func_800EAE8C(Obj *self, Values *values) {
    s32 blocked = 0;
    VTable *v;
    if (func_800E1CD4(self, 15) != 0 ||
        (v=self->vtable_24, v->action_94((char *)self + v->delta_90, 2, 9, 0, 0) != 0)) blocked=1;
    if (blocked) return 0;
    func_800E43EC(self, values->field_13, values->field_17);
    v=self->vtable_24;
    v->reset_64((char *)self + v->delta_60);
    func_800A533C((u8 *)self);
    return 1;
}
