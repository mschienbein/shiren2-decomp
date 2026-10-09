#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x, y; } Pair;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct { s16 delta, index; s32 (*call)(void *, s32, s32, u8, s32); } ActionSlot;
typedef struct { u8 pad_00[0x90]; ActionSlot action; } VTable;
typedef struct {
    Pair pos; ShirenDirection direction;
    u8 pad_09[0x15]; u8 flags_1E; u8 pad_1F[5]; VTable *vtable_24;
    u8 pad_28[0x61]; u8 strength_89;
} Actor;
/* The effect result is the pointer at +0x40; effect records occupy 0x50 bytes. */
typedef struct { u8 pad_00[0x40]; Actor *target; Pair pos; u8 pad_4C[4]; } Effect;
/* Shared 0x18-byte damage record, also used by func_800A00C4. */
typedef struct { void *source; u32 kind, amount; u16 strength, flags; u8 mode; u8 remaining[7]; } Damage;
extern u32 D_8013960C;
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Actor *obj);
extern void func_800497F0(s32 id, ...);
extern void *func_800C4360(void *self, void *owner, u16 value, s32 command, void *position, ShirenDirection direction, s32 limit, u16 flags, u8 mode);
extern void func_800C2D0C(Effect *effect);
extern void func_80136910(Damage *damage, void *source, u32 strength, u32 kind, u32 flags);
extern void func_800A7ADC(Actor *target, Damage *damage);
extern u16 func_800E08B0(void *obj);

static inline Damage *make_damage(Damage *damage, Actor *source) {
    func_80136910(damage, source, source->strength_89, 2, 0x2009);
    return damage;
}

static inline Pair *actor_position(Actor *actor) {
    return &actor->pos;
}

static inline ShirenDirection actor_direction(Actor *self) {
    return self->direction;
}

/* Monster action slot +0xB4 supplies a target; this override selects its own. */
s32 func_80106210(Actor *self, void *supplied_target) {
    Effect effect;
    Damage damage;
    Pair pos;
    Actor *target;
    s32 message = func_80049CB4(0x106C, self);
    func_800497F0(0x14E, message, func_800A3B20(self));
    D_8013960C <<= 1;
    func_800C4360(&effect, self, 0, 0x101, actor_position(self), actor_direction(self), 0xFF, 0x2011, 0);
    func_800C2D0C(&effect);
    D_8013960C >>= 1;
    target = effect.target;
    if (target != 0) {
        func_800A7ADC(target, make_damage(&damage, self));
        pos.x = target->pos.x;
        pos.y = target->pos.y;
        func_80049CB4(0x117, &pos);
        if ((target->flags_1E & 0x7C) && func_800E08B0(target)) {
            func_80049CB4(0x132);
            target->vtable_24->action.call((u8 *)target + target->vtable_24->action.delta, 0, 13, 0xFE, 0);
        }
    }
    return 1;
}
