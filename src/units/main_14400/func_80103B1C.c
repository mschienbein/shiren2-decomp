#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { short offset; short pad; void *fn; } VEntry;
typedef struct { unsigned char x, y, z, w; } Pos;
typedef struct { u32 pad0 : 16; u32 reflect : 1; u32 pad1 : 15; } Flags;
typedef struct { char pad[0x20]; Flags flags; VEntry *vtbl; } Actor;
s32 func_800F1040(Actor*, Actor*, s32);
s32 func_80049CB4(s32, ...);
char *func_800A3B20(void *obj);
void func_80049AE8(s32 message_id, ...);
s32 func_800A692C(Actor*, s32);
void func_800497F0(s32 message_id, ...);
void *func_800A65E4(void *out_direction, void *obj, void *target);
void func_800A665C(Actor*, Pos*);
void func_800E3678(Actor*, Actor*);
s32 func_800E0F40(void *obj);
u16 func_800E08B0(Actor*);
s32 func_800E1CC4(Actor*, s32);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);
/* D_8015BA78+0x6C binds the u32-returning func_800E0E88. */
#define VCALL_STAT(a) ((u32 (*)(void*))(a)->vtbl[13].fn)((char*)(a) + (a)->vtbl[13].offset)
s32 func_80103B1C(Actor *user, Actor *target){
    s32 result;
    s32 msg;
    Actor *victim;
    struct { Flags flags; Pos pos; } local;
    Pos *ppos;
    s32 before;
    s32 after;
    s32 atLimit;
    s32 reflect;
    result = func_800F1040(user, target, 0x40);
    switch (result) {
    case 2: return 1;
    case 1: return 0;
    }
    msg = func_80049CB4(0x40, user);
    func_80049CB4(6);
    func_80049AE8(0x145, msg, func_800A3B20(user));
    func_80049CB4(7);
    if (func_800A692C(user, 10)) {
        func_800497F0(0x225, msg);
    } else {
        victim = target;
        reflect = victim->flags.reflect;
        local.flags = victim->flags;
        if (reflect) {
            ppos = &local.pos;
            func_800A65E4(ppos, victim, user);
            func_800A665C(victim, ppos);
            func_80049CB4(0x1065, victim);
            func_80049CB4(6);
            func_80049AE8(0x50, msg, func_800A3B20(victim));
            func_80049CB4(7);
            victim = user;
        }
        func_80049CB4(0x23, victim, 0, 0x8000);
        func_800E3678(target, user);
        switch ((u8)func_800E0F40(user)) {
        case 1:
            before = (u16)VCALL_STAT(victim);
            atLimit = func_800E1CC4(victim, 7) == 1;
            if (!atLimit && before >= 2)
                ((s32 (*)(void*, s32, s32, u8, s32))victim->vtbl[18].fn)((char*)victim + victim->vtbl[18].offset, 0, 7, 0xFE, 0);
            after = (u16)VCALL_STAT(victim);
            break;
        case 2:
            before = func_800E08B0(victim);
            atLimit = func_800E1CC4(victim, 6) == 1;
            if (!atLimit)
                ((s32 (*)(void*, s32, s32, u8, s32))victim->vtbl[18].fn)((char*)victim + victim->vtbl[18].offset, 0, 6, 0xFE, 0);
            after = func_800E08B0(victim);
            break;
        default:
            before = (u8)func_800E0F40(victim);
            atLimit = func_800E1CC4(victim, 8) == 1;
            if (!atLimit && before >= 2)
                ((s32 (*)(void*, s32, s32, u8, s32))victim->vtbl[18].fn)((char*)victim + victim->vtbl[18].offset, 0, 8, 0xFE, 0);
            after = (u8)func_800E0F40(victim);
            break;
        }
        if (before == after) return 1;
    }
    func_800A08D8(1, msg, 0);
    return 1;
}
