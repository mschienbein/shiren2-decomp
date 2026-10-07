#include "common.h"

typedef unsigned char u8;
typedef struct { short offset; short pad; void *fn; } VEntry;
typedef struct { u32 pad0 : 28; u32 noStatus : 1; u32 pad1 : 3; } Flags;
typedef struct { char pad[0x1E]; u8 status; char pad1F; Flags flags; VEntry *vtbl; } Actor;
typedef struct { char pad[8]; VEntry *vtbl; signed char fC; signed char fD; } Item;
typedef struct { unsigned char x, y, z, w; } Pos;
extern u32 D_8013960C;
extern char D_80147620[];
s32 func_800A65B8(Actor*, Actor*);
void *func_800A65E4(Pos*, Actor*, Actor*);
s32 func_800A4754(Actor*, Actor*, Pos*);
s32 func_8010BEC4(Item*, u8);
void func_800E44EC(Actor*);
char *func_800A3B20(Actor*);
void func_800498E4(s32, ...);
s32 func_80049CB4(s32, ...);
void func_800A7B18(Actor*, Actor*, s32, s32);
void func_800EB744(Actor*, s32);
void func_800E9990(Actor*, s32, s32);
s32 func_8010BAE8(Item*, s32);
s32 func_800E0534(Actor*, s32);
s32 func_800C587C(void*, u8);
#define ITEM_STAT(it, id) ((u8 (*)(void*, s32))(it)->vtbl[8].fn)((char*)(it) + (it)->vtbl[8].offset, id)
#define ACTOR_IS_DEAD(a) ((s32 (*)(void*))(a)->vtbl[2].fn)((char*)(a) + (a)->vtbl[2].offset)
#define ACTOR_ADD_STATUS(a, s) ((s32 (*)(void*, s32, s32, u8, s32))(a)->vtbl[18].fn)((char*)(a) + (a)->vtbl[18].offset, 0, s, 0xFE, 0)
static inline s32 isNormalAttack(s32 kind) { return !(kind >= 7 && kind <= 19); }
static inline s32 canProc(Item *self, Actor *target) { return (target->status & 0x7C) && (u8)func_8010BEC4(self, 0x19); }
void func_8010D178(Item *self, Actor *owner, Actor *target, short damage, s32 kind){
    s32 doExtra;
    s32 hit;
    s32 amount;
    s32 pct;
    u32 chance;
    s32 sum;
    s32 total;
    s32 proc;
    s32 noStatus;
    struct { Pos pos; Flags flags; } local;
    Pos *ppos;
    if (damage <= 0) return;
    doExtra = 0;
    if (target) {
        doExtra = isNormalAttack(kind);
        if (ACTOR_IS_DEAD(target)) target = 0;
    }
    hit = 0;
    if (target) {
        if (kind == 1 && func_800A65B8(owner, target) == kind) {
            ppos = &local.pos;
            func_800A65E4(ppos, owner, target);
            hit = func_800A4754(owner, owner, ppos) != 0;
        }
        if (target) {
            if (kind == 1) {
                if (canProc(self, target)) func_800E44EC(target);
            }
            if (hit && (u8)func_8010BEC4(self, 0x5C)) {
                amount = damage - damage * ITEM_STAT(self, 0x5C) / 100;
                if (amount <= 0) amount = 1;
                func_800498E4(0x4F, func_800A3B20(owner));
                func_80049CB4(6);
                func_80049CB4(0x63, target);
                func_80049CB4(7);
                func_800A7B18(target, owner, amount, 4);
                if (ACTOR_IS_DEAD(target)) target = 0;
            }
        }
    }
    if (!doExtra) return;
    if ((owner->status >> 2) & 1) {
        pct = ITEM_STAT(self, 0x6B);
        if (pct > 0) {
            pct = damage * pct / 100;
            if (pct == 0) pct = 1;
            func_800EB744(owner, pct);
        }
    }
    total = ITEM_STAT(self, 0x6C) + ITEM_STAT(self, 3) + ITEM_STAT(self, 6);
    func_800E9990(owner, damage * total / 10, 0);
    if ((u8)func_8010BEC4(self, 0x5D) && self->fC + self->fD > 0) func_8010BAE8(self, -1);
    D_8013960C <<= 1;
    sum = ITEM_STAT(self, 1) + ITEM_STAT(self, 2);
    if (sum) {
        func_80049CB4(0x129, 5);
        func_800E0534(owner, sum);
    }
    D_8013960C >>= 1;
    if (target && (target->status & 0x7C) && hit) {
        proc = 0;
        noStatus = target->flags.noStatus;
        local.flags = target->flags;
        if (!noStatus) proc = func_800C587C(D_80147620, ITEM_STAT(self, 0x1F)) != 0;
        if (proc) {
            func_80049CB4(0x132);
            ACTOR_ADD_STATUS(target, 10);
        }
    }
    chance = ITEM_STAT(self, 0xA1);
    if (chance > 100) chance = 100;
    if (func_800C587C(D_80147620, chance)) ACTOR_ADD_STATUS(owner, 9);
    if ((u8)func_8010BEC4(self, 10))
        ((void (*)(void*, short))owner->vtbl[15].fn)((char*)owner + owner->vtbl[15].offset, -1);
}
