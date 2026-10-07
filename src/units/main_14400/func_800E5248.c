#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Unit800E5248 Unit800E5248;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(Unit800E5248 *self, s32 a, s32 b, u8 c, s32 d);
} VtEntry800E5248;

struct Unit800E5248 {
    u8 pad0[0xA];
    u8 unkA;
    u8 padB[0x1F - 0xB];
    u8 unk1F;
    u8 pad20[4];
    VtEntry800E5248 *vtable;
    u8 pad28[0x35 - 0x28];
    u8 slots35[10];
    u8 pad3F[3];
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 pad45[0x58 - 0x45];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 unk73;
    u8 unk74;
    u8 unk75;
    u8 unk76;
};

s32 func_800E0F40(Unit800E5248 *self);
void func_800E10C0(Unit800E5248 *self);
u32 func_800E10D0(Unit800E5248 *self);
void func_800E10FC(Unit800E5248 *self);
void func_800E1138(Unit800E5248 *self);
s32 func_800E1CD4(Unit800E5248 *self, s32 arg);
s32 func_800E1DA0(Unit800E5248 *self);
void func_800E4470(Unit800E5248 *self);
void func_800E4CF0(Unit800E5248 *self);

void func_800E5248(Unit800E5248 *self) {
    s32 i;

    for (i = 9; i != -1; i--) {
        if (self->slots35[i] == 0xFF) {
            self->slots35[i] = 0;
        }
    }
    if (self->unk42 == 0xFF || func_800E10D0(self) == 0x10) {
        s32 failed = func_800E1DA0(self) != 1;
        if (failed) {
            self->unk42 = 0;
            func_800E10C0(self);
        }
    }
    if (self->unk43 == 0xFF) {
        self->unk43 = 0;
        func_800E10FC(self);
    }
    if (self->unk44 == 0xFF) {
        self->unk44 = 0;
        func_800E1138(self);
    }
    func_800E4470(self);
    if (func_800E1CD4(self, 0xF)) {
        self->unk1F = 0xD6;
    } else if (self->vtable[18].func((Unit800E5248 *)((u8 *)self + self->vtable[18].delta), 2, 9, 0, 0)) {
        self->unk1F = 0xD5;
    } else {
        self->unk1F = self->unkA;
    }
    func_800E4CF0(self);
    self->unk58 = 0;
    self->unk5C = 0;
    self->unk60 = 0;
    self->unk64 = 0;
    self->unk68 = 0;
    self->unk6C = 0;
    self->unk71 = 0;
    self->unk70 = 0;
    self->unk72 = 0;
    self->unk73 = 3;
    self->unk74 = 0;
    self->unk75 = (u8)func_800E0F40(self);
    self->unk76 = 0;
}
