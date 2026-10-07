#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 type_0;
    s32 pad4[3];
    s32 arg_10;
    s32 pad14;
} Msg800C8A38;

typedef struct {
    u8 pad0[0x58];
    s16 delta_58;
    s16 pad5A;
    s32 (*func_5C)(void *self, Msg800C8A38 *msg);
} VTable800C8A38;

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F[0x5];
    VTable800C8A38 *vtable_24;
} Obj800C8A38;

extern Obj800C8A38 *D_801476B8;
extern u16 D_8014767C;

Obj800C8A38 *func_800C5F60(void);
u16 func_800E08B0(Obj800C8A38 *obj);
void func_80049BF0(s32 a0);
void func_800EBCD0(Obj800C8A38 *obj);
s32 func_80046240(void);

void func_800C8A38(u16 turns) {
    Msg800C8A38 msg;
    Msg800C8A38 *p;
    Obj800C8A38 *self = func_800C5F60();
    s32 flag = 0;
    s32 bit;

    if (func_800E08B0(self) == 0) {
        bit = (self->flags_1E >> 2) & 1;
        flag = bit == 0;
    }
    if (flag) {
        func_80049BF0(1);
        func_800EBCD0(D_801476B8);
    }
    flag = 0;
    if (func_80046240() != 0 || ((D_8014767C >> 6) & 1)) {
        flag = 1;
    }
    if (flag) {
        return;
    }
    flag = 1;
    if ((D_8014767C >> 7) & 1) {
        flag = (D_8014767C >> 8) & 1;
        flag ^= 1;
    }
    p = &msg;
    if (flag) {
        msg.type_0 = 2;
        D_801476B8->vtable_24->func_5C((u8 *)D_801476B8 + D_801476B8->vtable_24->delta_58, p);
        if (func_800E08B0(D_801476B8) == 0) {
            return;
        }
    }
    msg.type_0 = 3;
    p->arg_10 = (u8)turns;
    D_801476B8->vtable_24->func_5C((u8 *)D_801476B8 + D_801476B8->vtable_24->delta_58, p);
    if (func_800E08B0(D_801476B8) == 0) {
        return;
    }
    if (self == D_801476B8) {
        return;
    }
    if (flag) {
        msg.type_0 = 2;
        self->vtable_24->func_5C((u8 *)self + self->vtable_24->delta_58, p);
        if (func_800E08B0(self) == 0) {
            return;
        }
    }
    msg.type_0 = 3;
    p->arg_10 = (u8)turns;
    self->vtable_24->func_5C((u8 *)self + self->vtable_24->delta_58, p);
}
