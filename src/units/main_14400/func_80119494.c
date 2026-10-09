#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0xA]; u8 kind_0A; u8 pad_0B[0x11]; u16 flags_1C; u8 flags_1E; } Actor;
typedef struct { u8 pad_00[0x40]; short adjustment_40, unused_42; void (*apply_44)(void *, Actor *); } Table;
typedef struct { u8 pad_00[8]; Table *vtable_08; } Object;
extern u16 func_800E08B0(void *);
extern s32 func_800E1CD4(void *, s32);
extern s32 func_800E1CC4(void *, s32);
/* Item vtable slot +0x4C pair action (self, source, target); the supplied source unit is unused here. */
void func_80119494(Object *self, void *unused, Actor *target) {
    s32 candidate = 0;
    if ((target->flags_1E & 0xC) || target->kind_0A == 0x57 || target->kind_0A == 0x5A || target->kind_0A == 0x5E || target->kind_0A == 0x58) candidate = 1;
    if (candidate) {
        s32 allowed = 0;
        if (func_800E08B0(target) && !func_800E1CD4(target, 0xA) && !func_800E1CD4(target, 0xB) && !func_800E1CD4(target, 0xC) && !func_800E1CD4(target, 0xD) && !func_800E1CD4(target, 0xE) && !func_800E1CD4(target, 0xF) && !(target->flags_1C & 1) && (!((target->flags_1E >> 2) & 1) || !func_800E1CC4(target, 2))) allowed = 1;
        if (allowed) self->vtable_08->apply_44((u8 *)self + self->vtable_08->adjustment_40, target);
    }
}
