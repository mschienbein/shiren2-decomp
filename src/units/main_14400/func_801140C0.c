#include "common.h"
typedef unsigned char u8;
typedef struct S S;
typedef struct { u8 pad_0[0x18]; short delta_18, index_1A; void (*set_1C)(void *self, s32 value); } ListVTable;
typedef struct { void *field_0; ListVTable *vtable_4; u8 pad_8[0x14]; } List;
typedef struct { u8 pad_0[8]; const void *vtable_8; List list_C; u8 field_28, field_29; } Obj801216D0;
extern void *func_800AC0C0(S *self, s32 a, s32 b);
extern List *func_800CFF00(List *self, void *owner, u8 count);
extern void *func_8011422C(u8 *self);
extern const u8 D_8015D938[];
/* D_80154550 (item-set family D_80154300) slot +0x1C is func_800CE6F8: void (void *self, s32 value). */
Obj801216D0 *func_801140C0(Obj801216D0 *self, s32 kind, s32 arg) {
    List *list;
    ListVTable *v;
    /* Only the low byte of the signed-int argument is used (andi 0xFF at 0x801140F0). */
    u8 count = (u8)arg;
    func_800AC0C0((S *)self, 9, kind);
    self->vtable_8 = D_8015D938;
    func_800CFF00(&self->list_C, self, count);
    self->field_29 = 0;
    self->field_28 = 1;
    list = func_8011422C((u8 *)self);
    v = list->vtable_4;
    v->set_1C((char *)list + v->delta_18, count);
    return self;
}
