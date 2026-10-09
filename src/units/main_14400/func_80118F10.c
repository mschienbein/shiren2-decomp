#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u32 high:28; u32 calm:1; u32 low:3; } Flags;
typedef struct { u8 pad_00[0x90]; short delta_90, index_92; s32 (*action_94)(void *, s32, s32, u8, s32); } Methods;
typedef struct { Pos position; u8 pad_08[0x14]; u16 flags_1C; u8 pad_1E[2]; Flags flags_20; Methods *table_24; } Actor;
extern u32 D_8013960C;
extern u8 D_80147620[];
extern u8 D_801F5228[32];
extern u16 func_800E08B0(void *);
extern s32 func_800E1CD4(void *, s32);
extern s32 func_800A6E90(void *);
extern s32 func_800C5844(void *, u8, u8);
extern void func_800E20F0(void *);
extern char *func_800A3B20(void *);
extern void func_80049A04(u16, ...);
extern s32 func_80049CB4(s32, ...);
extern s32 func_800A08D8(s32, s32, s32);
extern s32 func_800A7BE4(void *, void *);
extern void func_800A7B18(void *, void *, s32, s32);
extern u32 func_800E110C(void *);
/* ODD_C: value helpers retain aggregate snapshots and shared virtual-call tails. */
static inline void copyPosition(Pos *out, Pos *in) { out->x = in->x; out->y = in->y; }
static inline s32 isCalm(Flags *flags) { return flags->calm; }
static inline s32 act(Actor *actor, s32 mode, s32 kind, u8 value, s32 amount) {
    return actor->table_24->action_94((u8 *)actor + actor->table_24->delta_90, mode, kind, value, amount);
}
static inline void notify(Actor *actor, s32 mode, s32 kind, u8 value, s32 amount) {
    D_8013960C <<= 1;
    act(actor, mode, kind, value, amount);
    D_8013960C >>= 1;
}
/* The item receiver is supplied but this operation only uses source and target. */
s32 func_80118F10(void *unused, Actor *source, Actor *target, s32 count) {
    s32 blocked = 0;
    u32 enabled;
    s32 selected, remaining;
    Pos position;
    char *name;
    if (!func_800E08B0(target) || func_800E1CD4(target, 0xB) || func_800A6E90(target) ||
        func_800E1CD4(target, 0xF) || (target->flags_1C & 1)) blocked = 1;
    if (blocked) return 0;
    enabled = D_801F5228[4];
    remaining = func_800C5844(D_80147620, 1, (u8)count);
    for (selected = 1; ; selected <<= 1) {
        s32 active = enabled & selected;
        if (active) {
            s32 next = remaining - 1;
            remaining = next;
            next &= 0xFF;
            if (!next) break;
        }
    }
    func_800E20F0(target);
    name = func_800A3B20(target);
    copyPosition(&position, &target->position);
    switch (selected) {
    case 1:
        func_80049A04(0x8B, name);
        notify(target, 0, 0xC, 0xFE, 0);
        break;
    case 2:
        func_800A7BE4(target, source);
        func_80049A04(0x8C, name);
        func_80049CB4(0x128, 0xA0);
        if (func_800E1CD4(target, 0xC)) notify(target, 1, 0xC, 0, 0);
        break;
    case 4:
        func_80049A04(0x8D, name);
        func_800A08D8(1, -1, 0);
        func_800A7B18(target, source, (u8)func_800C5844(D_80147620, 0xA, 0x1E), 0x20);
        break;
    case 8: {
        Flags flags = target->flags_20;
        if (isCalm(&flags)) func_80049A04(0x8F, name);
        else { func_80049A04(0x8E, name); notify(target, 0, 0xA, 0xFE, 0); }
        break;
    }
    case 0x10:
        func_80049A04(0x90, name);
        notify(target, 0, 0x13, 0xFE, 0);
        break;
    case 0x20:
        func_80049A04(0x91, name);
        notify(target, 0, 0xD, 0xFE, 0);
        func_80049CB4(0x117, &position);
        break;
    case 0x40: {
        s32 allowed;
        func_80049A04(0x92, name);
        allowed = 0;
        if (!act(target, 2, 0x11, 0, 0) || (signed char)func_800E110C(target)) allowed = 1;
        if (allowed) { func_80049CB4(0x121, &position); notify(target, 0, 0x11, 0xFE, -3); }
        break;
    }
    }
    func_800A08D8(1, -1, 0);
    return 1;
}
