#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
typedef struct { s32 x, y; } Pair;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct Unit Unit;
/* Entity table at unit+0x24; slot +0x14 is the receiver-only s32 query. */
typedef struct { u8 pad_00[0x10]; s16 delta_10; s16 index_12; s32 (*method_14)(void *self); } VTable;
struct Unit {
    /* 0x00 */ Pair pos;
    /* 0x08 */ ShirenDirection direction;
    /* 0x09 */ u8 pad_09[0x1C - 0x09];
    /* 0x1C */ u16 flags_1C;
    /* 0x1E */ u8 pad_1E[0x24 - 0x1E];
    /* 0x24 */ VTable *vtable_24;
    /* 0x28 */ u8 pad_28[0x58 - 0x28];
    /* 0x58 */ Unit *target_58;
    /* 0x5C */ u8 pad_5C[0x89 - 0x5C];
    /* 0x89 */ u8 strength_89;
    /* 0x8A */ u8 pad_8A[0x9A - 0x8A];
    /* 0x9A */ u16 flags_9A;
};
/* The effect result is the pointer at +0x40; effect records occupy 0x50 bytes. */
typedef struct { u8 pad_00[0x40]; Unit *target; Pair pos; u8 pad_4C[4]; } Effect;
/* Shared 0x18-byte damage record. */
typedef struct { void *source; u32 kind, amount; u16 strength, flags; u8 mode; u8 remaining[7]; } Damage;

extern SelectionRecord D_80142F18;
extern u32 D_8013960C;

extern s32 func_800E20CC(void *arg0);
extern s32 func_800E0F40(Unit *obj);
extern Unit *func_800F1A58(Unit *a, s32 b, s32 c);
extern u8 func_800A6420(Unit *obj, Unit *target);
extern s32 func_800A4520(void *ctx, Unit *obj);
extern s32 func_800F10F8(Unit *object, void *target, s32 force, s32 apply, s32 override);
extern s32 func_800F1244(Unit *arg, Unit *target, s32 distance, s32 force);
extern s32 func_800A58B8(Unit *self);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800E1CC4(Unit *obj, s32 kind);
extern void *func_800C4360(void *self, void *owner, u16 value, s32 command, void *position, ShirenDirection direction, s32 limit, u16 flags, u8 mode);
extern void func_800C2D0C(Effect *effect);
extern char *func_800A3B20(Unit *u);
extern void func_800497F0(s32 id, ...);
extern void func_80136910(Damage *damage, void *source, u32 strength, u32 kind, u32 flags);
extern void func_800A7ADC(Unit *target, Damage *damage);

static inline s32 is_special_floor(void) { return D_80142F18.mode == 0x4F; }

static inline Damage *make_damage(Damage *damage, Unit *source) {
    func_80136910(damage, source, source->strength_89, 2, 0x809);
    return damage;
}

static inline ShirenDirection actor_direction(Unit *unit) {
    return unit->direction;
}

s32 func_800F9094(Unit *self, Unit *target) {
    Pair origin;
    Pair at;
    s32 result;
    s32 special = func_800E20CC(self) || is_special_floor();

    if (special) {
        if ((u8)func_800E0F40(self) >= 2) {
            s32 drop;
            target = func_800F1A58(self, 0x38, 0);
            drop = func_800A6420(self, target) == 3 && (u8)func_800E0F40(self) == 2;
            if (drop) {
                target = 0;
            }
        }
    } else {
        Unit *current = self->target_58;
        s32 none = current == 0 ||
                   current->vtable_24->method_14((u8 *)current + current->vtable_24->delta_10);
        if (none) {
            return func_800A4520(self, target) == 0;
        }
        target = self->target_58;
    }
    switch ((u8)func_800E0F40(self)) {
    case 1:
        {
            s32 bit = self->flags_9A & 0x40;
            result = func_800F10F8(self, target, 0, bit != 0, 0);
        }
        break;
    case 2:
        if (func_800E20CC(self) && target == 0) {
            result = 0;
        } else {
            result = func_800F1244(self, target, 0x4C, 1);
        }
        break;
    default:
        if (self->flags_9A & 0x40) {
            result = func_800F1244(self, target, 0x4C, 1);
        } else {
            result = 0;
        }
        break;
    }
    switch (result) {
    case 1:
        return 0;
    case 2:
        return 1;
    case 0:
        if (target != 0 && func_800A58B8(target) != 2) {
            return 1;
        }
        break;
    }
    origin.x = self->pos.x;
    origin.y = self->pos.y;
    func_80049CB4(0x106B, self);
    {
        s32 near = (u8)func_800E0F40(self) == 1 || func_800E1CC4(self, 4);
        if (near || target == 0) {
            /* The effect record lives in its own scope; its frame slot is reused below. */
            {
                Effect effect;
                D_8013960C <<= 1;
                func_800C4360(&effect, self, 0, 0x100, &origin, actor_direction(self), 0xFF, 0x811, 0);
                func_800C2D0C(&effect);
                target = effect.target;
                D_8013960C >>= 1;
                if (target == 0) {
                    return 1;
                }
                at = target->pos;
            }
        } else {
            Pair pos;
            pos.x = target->pos.x;
            pos.y = target->pos.y;
            func_80049CB4(0x100, &origin, &pos);
        }
    }
    {
        s32 ready = (target->flags_1C & 1) ^ 1;
        if (ready) {
            Damage damage;
            s32 message = func_80049CB4(0xDA, &at);
            func_800497F0(0x11E, message, func_800A3B20(target));
            func_800A7ADC(target, make_damage(&damage, self));
        }
    }
    return 1;
}
