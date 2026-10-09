#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 kind;
    u8 pad4[0x10];
} Message800EBCD0;

typedef struct {
    u8 pad0[0x58];
    s16 delta_58;
    s16 index_5A;
    s32 (*receive_5C)(void *self, Message800EBCD0 *message);
    s16 delta_60;
    s16 index_62;
    void (*method_64)(void *self);
} UnitVTable800EBCD0;

typedef struct Unit800EBCD0 {
    u8 pad0[0xA];
    u8 kind_A;
    u8 padB[0x24 - 0xB];
    UnitVTable800EBCD0 *vtable;
    u8 pad28[0x58 - 0x28];
    struct Unit800EBCD0 *partner_58;
    u8 pad5C[0x6C - 0x5C];
    s32 field_6C;
    u8 pad70[0x104 - 0x70];
    struct Unit800EBCD0 *partner_104;
} Unit800EBCD0;

extern u16 D_8014767C;
extern s32 D_80147678;

extern u16 func_800E08B0(void *obj);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_800E2298(void *arg0);
extern char *func_800A3CD0(Unit800EBCD0 *o);
extern void func_800E1048(Unit800EBCD0 *o);
extern void func_800C94E8(void);
extern s32 func_800E1DA0(Unit800EBCD0 *obj);
extern void func_800EC68C(Unit800EBCD0 *record, u32 value);

void func_800EBCD0(Unit800EBCD0 *self) {
    Unit800EBCD0 *partner;
    Message800EBCD0 message;

    if (self->partner_104 == 0) {
        return;
    }
    self->partner_104->partner_58 = 0;
    partner = self->partner_104;
    self->partner_104 = 0;
    if (func_800E08B0(partner)) {
        func_80049CB4(0x1F, partner);
    }
    if (partner->kind_A == 0x29) {
        message.kind = 2;
        partner->vtable->receive_5C((u8 *)partner + partner->vtable->delta_58, &message);
    }
    if (D_8014767C & 3) {
        func_800E2298(self);
        func_80049CB4(0x1F, self);
        func_80049CB4(0x93, self);
        func_80049CB4(0xDD);
        func_800498E4(0xE9, func_800A3CD0(self));
        func_800E1048(self);
    }
    self->vtable->method_64((u8 *)self + self->vtable->delta_60);
    func_80049CB4(0x89, self);
    func_800C94E8();
    if (func_800E1DA0(self)) {
        func_800EC68C(self, 0x1D);
        self->field_6C = 0;
        D_80147678 = 1;
        func_800498E4(0xC4);
        func_80049CB4(0x129, 0x32);
    }
}
