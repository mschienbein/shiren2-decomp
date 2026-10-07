#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Sub80114A50 Sub80114A50;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(Sub80114A50 *self);
} VtEntry80114A50;

struct Sub80114A50 {
    u8 pad0[4];
    VtEntry80114A50 *vtable;
};

typedef struct {
    u8 unk0;
    u8 id;
    u8 pad2[3];
    s8 unk5;
    u8 pad6[6];
    Sub80114A50 sub;
} Item80114A50;

typedef struct {
    s32 index;
    void *list;
    s32 reverse;
    void *current; /* +0xC, written by func_800CEBA0 and read by func_800CEC68. */
} Iter80114A50;

extern u16 D_8015775C[];
extern u8 D_80157788[];
u8 func_800AE98C(Item80114A50 *self);
s32 func_800AC584(u16 base);
void *func_8011422C(Item80114A50 *self);
Iter80114A50 *func_800CEB20(Iter80114A50 *it, void *list);
s32 func_800CEBA0(Iter80114A50 *it);
void *func_800CEC68(Iter80114A50 *it);
s32 func_800AEC2C(void *item);

u32 func_80114A50(Item80114A50 *self) {
    u8 kind = func_800AE98C(self);
    u32 base = (u32)func_800AC584(D_8015775C[kind]);
    Sub80114A50 *sub = &self->sub;
    u32 scale = (u32)sub->vtable[2].func((Sub80114A50 *)((u8 *)sub + sub->vtable[2].delta));
    u32 bonus = base * D_80157788[kind] * scale / 100;
    u32 total = 0;
    /* unk5 != -1, compared against the zeroed total (reuses its register) */
    u8 known = (u32)~self->unk5 > total;

    if (!known) {
        total = base + bonus;
    }
    if (self->id == 0xAB) {
        if (!known) {
            return base;
        }
        return 0;
    } else {
        Iter80114A50 it;

        func_800CEB20(&it, func_8011422C(self));
        while (func_800CEBA0(&it)) {
            total += (u32)func_800AEC2C(func_800CEC68(&it));
        }
        return total;
    }
}
