#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct VTable800FCA90 VTable800FCA90;

typedef struct {
    char pad0[0x24];
    VTable800FCA90 *vtable;
    char pad28[0x89 - 0x28];
    u8 field_89;
    char pad8A[0x9A - 0x8A];
    u16 flags_9A;
    char pad9C[2];
    u8 field_9E;
} Obj800FCA90;

extern VTable800FCA90 D_8015A5F0;

extern Obj800FCA90 *func_800EFC70(Obj800FCA90 *obj, s32 arg1, u8 arg2);

Obj800FCA90 *func_800FCA90(Obj800FCA90 *self, u8 kind) {
    func_800EFC70(self, 0x28, kind);
    self->vtable = &D_8015A5F0;
    self->field_9E = self->field_89;
    self->flags_9A |= 8;
    return self;
}
