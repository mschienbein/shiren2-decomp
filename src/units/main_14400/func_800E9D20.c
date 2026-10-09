#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_0[6]; u16 field_6; } Target;
typedef struct { u8 pad_0[0xA0]; short delta_A0, index_A2; Target *(*get_A4)(void *, u8); } VTable;
typedef struct {
    u8 pad_0[0xA]; u8 kind_A; u8 pad_B[0x19]; VTable *vtable_24;
    u16 field_28, field_2A, field_2C, field_2E; u8 pad_30[2];
    u8 field_32; u8 pad_33[0x45]; s32 field_78;
} S;
extern s32 func_800EAFB8(void *self, s32 flags);
/* Slot 0xA4 includes func_800EE060 -> func_80044C60. */
void func_800E9D20(S *p) {
    p->field_32 = 1;
    p->field_78 = 0;
    if (p->kind_A == 0x1C) {
        VTable *v = p->vtable_24;
        Target *target = v->get_A4((char *)p + v->delta_A0, 1);
        p->field_2A = target->field_6;
        p->field_28 = target->field_6;
    } else {
        p->field_2A = 15;
        p->field_28 = 15;
    }
    p->field_2E = 8;
    p->field_2C = 8;
    func_800EAFB8(p, 0);
}
