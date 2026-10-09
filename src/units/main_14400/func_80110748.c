#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;
typedef struct { s16 offset; s16 field_2; void (*method)(void); } Method;
typedef struct { u8 value; } Dir;
typedef struct { u32 field_0 : 28; u32 blocked : 1; u32 field_1D : 3; } WordFlags;
typedef struct { u8 field_0[8]; Method *field_8; s8 field_C, field_D; u8 field_E[0x13]; u8 field_21; } Actor;
typedef struct { u8 field_0[0x1E]; u8 field_1E, field_1F; WordFlags field_20; Method *field_24; u8 field_28[0x54]; u16 field_7C; } Target;
typedef struct { Target *target; u8 flag; u8 field_5[3]; } Hit;
typedef struct { s32 x, y; } Pos;
/* func_801106BC supplies the 0x28-byte header followed by sixteen hit records. */
typedef struct { u8 field_0[4]; Pos field_4; Dir field_C; u8 field_D[3]; Pos field_10; u32 field_18; u8 field_1C[2]; u8 field_1E, field_1F, field_20, field_21, field_22, field_23, field_24, field_25, field_26, field_27; Hit hits[16]; } Attack;
typedef struct { void *field_0; s32 field_4, field_8; s16 field_C; u16 field_E; u8 field_10; } Damage;
typedef struct { u16 flags, amount; s32 field_4; } Outcome;
extern u32 D_8013960C;
extern u8 D_80147620[];
extern u8 D_80156987, D_80156A13;
extern u16 D_80156990;
extern void func_80049A04(u16, ...);
extern s32 func_80049CB4(s32, ...);
extern void func_800A7A9C(Target *, Damage *, Outcome *), func_800A7BA4(void *, s32);
extern char *func_800AC990(Actor *);
extern s32 func_800C587C(void *, u8);
extern s32 func_800E0534(void *, s32);
extern u16 func_800E08B0(void *);
extern s32 func_800E8DC8(void *, s16);
extern s32 func_8010B9CC(Actor *, u8, u8);
extern s32 func_8010BAE8(Actor *, s32);
extern s32 func_8010BEC4(Actor *, u8);
extern u32 func_8010EAF4(Actor *);
extern s32 func_8010EBA4(Actor *), func_8010F580(Actor *, void *, Attack *);
extern void func_8010F7B4(Actor *, void *, Attack *), func_8010F8A0(Actor *, void *, Attack *);
extern s32 func_8010F838(Actor *, void *, void *);
extern void func_8010F9FC(Actor *, void *, Dir, void *, Pos *), func_8010FB30(Actor *, Attack *);
extern s32 func_8010FF20(Actor *, void *, Attack *);
extern void func_80136910(Damage *, void *, u32, u32, u32);
static inline u8 stat(Actor *self, s32 id) { return ((u8 (*)(void *, s32))self->field_8[8].method)((u8 *)self + self->field_8[8].offset, id); }
static inline s32 bit_set(u32 value, s32 bit) { return (value >> bit) & 1; }
static inline s32 blocked_flag(WordFlags *flags) { return flags->blocked; }
/* Damage record initializer: func_80136910 with kind 1 and no flags. */
static inline void init_damage(Damage *damage, void *source, s16 amount) { func_80136910(damage, source, amount, 1, 0); }
static inline void scale_damage(Damage *damage, s32 scale) {
    s32 amount = damage->field_C * scale / 10;
    if (amount <= 0x7FFF) damage->field_C = amount;
    else damage->field_C = 0x7FFF;
}
static inline void scale_damage_at(Damage *damage, const u8 *scale) {
    s32 amount = damage->field_C * *scale / 10;
    if (amount <= 0x7FFF) damage->field_C = amount;
    else damage->field_C = 0x7FFF;
}
static inline void add_flag(Damage *damage, u16 flag) { damage->field_E |= flag; }
static inline void announce_mode(Actor *self, void *context, Attack *attack, s32 mode) {
    switch (mode) {
    case 1: func_80049CB4(0x6E, context, 1); return;
    case 2:
    case 4: func_80049CB4(0x47, context, 1, 0); return;
    case 3: func_80049CB4(0x69, context, 1); return;
    default:
        if (attack->field_23) func_8010FB30(self, attack);
        else func_80049CB4(0x47, context, attack->field_22, 0);
        return;
    }
}
void func_80110748(Actor *self, void *context, Attack *attack) {
    Damage damage;
    Outcome outcome;
    WordFlags flags;
    s32 affected;
    Hit *entry;
    s32 remaining;
    u8 hit_flag;
    s32 mode = func_8010F580(self, context, attack);
    announce_mode(self, context, attack, mode);
    switch (mode) {
    case 1: func_8010F7B4(self, context, attack); return;
    case 4: { Dir dir = attack->field_C; func_8010F9FC(self, context, dir, &attack->field_4, &attack->field_10); return; }
    case 2: if (func_8010F838(self, context, &attack->field_10)) return; break;
    case 3: func_8010F8A0(self, context, attack); return;
    }
    affected = 0;
    entry = attack->hits;
    remaining = attack->field_26;
    for (;;) {
        Target *target;
        s32 first_stat;
        if (--remaining == -1) break;
        target = entry->target;
        hit_flag = entry->flag;
        ++entry;
        if (((s32 (*)(void *))target->field_24[2].method)((u8 *)target + target->field_24[2].offset)) continue;
        init_damage(&damage, context, func_8010EAF4(self));
        damage.field_C = func_800E8DC8(context, damage.field_C);
        if (bit_set(target->field_1E, 4)) {
            /* ODD_C: a second name for the target keeps local-alloc from merging it with the loop cursor. */
            Target *typed = target;
            if (bit_set(target->field_7C, 4)) scale_damage(&damage, (u8)stat(self, 0x3B));
            if (bit_set(typed->field_7C, 6)) {
                scale_damage(&damage, (u8)stat(self, 0x3D));
                scale_damage(&damage, (u8)stat(self, 0x53));
            }
            if (bit_set(typed->field_7C, 1)) scale_damage(&damage, (u8)stat(self, 0x3E));
            if (bit_set(typed->field_7C, 2)) scale_damage(&damage, (u8)stat(self, 0x3C));
            if (bit_set(typed->field_7C, 12)) scale_damage(&damage, (u8)stat(self, 0x56));
        }
        if (attack->field_1E || func_800C587C(D_80147620, (u8)stat(self, 0x39))) {
            scale_damage_at(&damage, &D_80156987);
            damage.field_E |= 0x80;
        } else if (bit_set(attack->field_18, 21)) {
            scale_damage_at(&damage, &D_80156A13);
            damage.field_E |= 0x100;
        }
        if ((u8)func_8010BEC4(self, 0x19)) add_flag(&damage, 0x200);
        if (attack->field_1F) add_flag(&damage, 1);
        first_stat = (u8)stat(self, 3);
        damage.field_10 = func_8010B9CC(self, first_stat, (u8)stat(self, 6));
        damage.field_8 = (u8)func_8010FF20(self, context, attack);
        {
            s32 kind = (s8)hit_flag;
            if (kind == 1) add_flag(&damage, 1);
            else if (kind == 0) add_flag(&damage, 2);
        }
        outcome.flags = 0;
        func_800A7A9C(target, &damage, &outcome);
        if (!func_800E08B0(context)) break;
        {
            s32 not_blocked = bit_set(outcome.flags, 3) ^ 1;
            if (not_blocked) {
                if (target->field_1E & 0x7C) {
                    s32 not_hit = (outcome.flags & 1) ^ 1;
                    if (not_hit) {
                        s32 vulnerable;
                        flags = target->field_20;
                        vulnerable = blocked_flag(&flags) ^ 1;
                        if (vulnerable && func_800C587C(D_80147620, (u8)stat(self, 0x1F))) {
                            func_80049CB4(0x132);
                            ((s32 (*)(void *, s32, s32, u8, s32))target->field_24[18].method)((u8 *)target + target->field_24[18].offset, 0, 10, 0xFE, 0);
                        }
                    }
                    if ((u8)func_8010BEC4(self, 0x51)) {
                        s32 recovery;
                        D_8013960C *= 2;
                        recovery = (s16)((s16)outcome.amount / 3);
                        if (recovery <= 0) recovery = 1;
                        func_800E0534(context, recovery);
                        D_8013960C >>= 1;
                    }
                    affected = 1;
                }
                if ((u8)func_8010BEC4(self, 0x48) && self->field_C + self->field_D > 0) func_8010BAE8(self, -1);
            } else {
                if ((u8)func_8010BEC4(self, 0x4A) && self->field_21 < D_80156990) ++self->field_21;
            }
        }
    }
    if (func_800E08B0(context)) {
        if (affected) {
            u8 first, second;
            s32 recovery;
            if (attack->field_1E) { self->field_21 = 0; func_80049CB4(0x8E, context, self); }
            first = ((u8 (*)(void *, s32))self->field_8[8].method)((u8 *)self + self->field_8[8].offset, 1);
            second = ((u8 (*)(void *, s32))self->field_8[8].method)((u8 *)self + self->field_8[8].offset, 2);
            recovery = first + second;
            if (recovery > 0) {
                D_8013960C *= 2;
                func_800E0534(context, recovery);
                D_8013960C >>= 1;
            }
        }
        if (!attack->field_1E && func_8010EBA4(self)) {
            func_80049CB4(0x8D, context, self);
            func_80049CB4(0x128, 0x57);
            func_80049A04(0xEA, func_800AC990(self));
        }
        if (attack->field_23) func_800A7BA4(context, 7);
    }
}
