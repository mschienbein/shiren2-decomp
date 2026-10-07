#include "common.h"
typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct { s16 delta; s16 index; void *(*fn)(void *); } VEntry;
typedef struct { char pad0[0xA]; u8 unkA; char padB[0x24 - 0xB]; VEntry *vtbl; } Self;
typedef struct { char pad0[0x10]; s32 unk10; } Arg;
extern u32 D_8013960C;
s32 func_800E3D20(Self *, s32);
void func_800E42AC(Self *, s32);
u16 func_800E08B0(Self *);
s32 func_800CF47C(void *);
void func_800AA5C0(s32);
void func_800EEE28(Self *self, Arg *arg) {
    s32 value = arg->unk10;
    void *res;
    D_8013960C = (D_8013960C << 1) | 1;
    func_800E3D20(self, value);
    func_800E42AC(self, value);
    D_8013960C >>= 1;
    if (func_800E08B0(self) == 0) {
        res = self->vtbl[19].fn((char *)self + self->vtbl[19].delta);
        if (res != 0) {
            func_800CF47C(res);
        }
        func_800AA5C0(self->unkA);
    }
}
