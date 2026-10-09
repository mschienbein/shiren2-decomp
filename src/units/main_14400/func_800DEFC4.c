#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_00[8]; Dir direction; u8 flags; } Action;
typedef struct {
    Pair pos; u8 direction; u8 pad_09; u8 kind_0A; u8 pad_0B[0x11];
    u16 flags_1C; u8 flags_1E; u8 pad_1F[0x81]; Pair destination_A0;
    u8 pad_A8[0x3C]; u16 flags_E4;
} Actor;
typedef struct { u8 pad_00; u8 kind; u8 pad_02[10]; u16 id; } Item;
extern u8 D_80147620[];
extern s32 D_80148090;

extern Actor *func_800C5F60(void);
extern s32 func_800E1D14(Actor *obj, s32 kind);
extern u8 func_800C57CC(void *rng, s32 limit);
extern void *func_800A7DEC(void *value);
extern s32 func_800A4CC4(void *self, void *pos, void *direction);
extern void func_800A2F80(Dir *direction, s32 amount);
extern void func_800A665C(Actor *self, u8 *direction);
extern void *func_800A2594(Pair *out, void *from, Dir direction);
extern void *func_800B4D80(Pair *pos);
extern s32 func_800E2044(Actor *self);
extern s32 func_801290F4(Item *item, Dir direction, s32 flag);
extern void func_801F212C(u16 id, u8 flag);
extern s32 func_80102F50(void *self, void *target);
extern s32 func_800A4754(Actor *obj, void *arg, Dir *direction);
extern s32 func_801032C4(Actor *self);
extern s32 func_800E1CD4(Actor *self, s32 kind);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void *func_800B4928(Pair *pos);
extern s32 func_800DF40C(Action *action, Actor *self, Dir direction, Actor *target);
extern s32 func_800E6030(Actor *self, Dir *direction);
/* The unused first parameter is the action/this pointer forwarded by this caller. */
extern void func_800DF61C(Action *action, Actor *obj, Pair *pos);
extern s32 func_800E1CC4(Actor *self, s32 kind);
extern void func_800D72E0(u8 *obj, s32 mode);
extern s32 func_800FCF3C(Pair *pos, s32 mode);
extern void func_800A4EC0(void *self, void *direction);
extern void func_8006D438(void);

static inline s32 remaining_attempts(s32 count) {
    return count;
}

static inline void copy_position(Pair *out, Pair *in) {
    out->x = in->x;
    out->y = in->y;
}

static inline s32 controls_disabled(SelectionRecord *record) {
    return (record->flags >> 2) & 1;
}

static inline s32 controls_enabled(SelectionRecord *record) {
    return controls_disabled(record) ^ 1;
}

static inline s32 direction_available(Actor *self, void *position, Dir *dir) {
    return func_800A4CC4(self, position, dir);
}

s32 func_800DEFC4(Action *action) {
    Pair origin, next;
    Dir trial, direction;
    Actor *self = func_800C5F60();
    Item *item;
    Actor *target;
    s32 changed = 0;
    s32 special, mode;
    direction = action->direction;
    if (func_800E1D14(self, 0x13) || func_800E1D14(self, 0x14)) changed = 1;
    if (changed) {
        s32 count;
        trial.value = func_800C57CC(D_80147620, 7) & 7;
        count = 8;
        while (remaining_attempts(--count) != -1) {
            if (direction_available(self, func_800A7DEC(self), &trial)) break;
            func_800A2F80(&trial, 1);
        }
        direction = trial;
    }
    func_800A665C(self, &direction.value);
    copy_position(&origin, &self->pos);
    func_800A2594(&next, &origin, direction);
    special = 0;
    item = func_800B4D80(&next);
    if (item && item->kind == 0xF4 && func_800E2044(self)) special = (self->flags_1E >> 2) & 1;
    if (special && func_801290F4(item, direction, 0)) {
        func_801F212C(item->id, 0);
        return 1;
    }
    if (!(self->flags_1C & 2)) {
        if (self->kind_0A == 0x42) {
            s32 can_move;
            func_800A665C(self, &direction.value);
            can_move = 0;
            if (func_80102F50(self, &next)) can_move = func_800A4754(self, &origin, &direction) != 0;
            if (can_move) {
                self->destination_A0 = next;
                if (func_801032C4(self)) return 2;
            }
        }
        if (func_800E1CD4(self, 0x10)) {
            if (func_800A4CC4(self, func_800A7DEC(self), &direction)) {
                func_80049CB4(0xEA, &origin);
                func_800498E4(0x1B9);
                return 0;
            }
            return 1;
        }
        target = func_800B4928(&next);
        {
            s32 can_swap = 0;
            if (target && (target->flags_1E & 0x7C)) can_swap = func_800DF40C(action, self, direction, target) != 0;
            if (can_swap && func_800E6030(self, &direction)) {
                func_800DF61C(action, target, &next);
                if (((self->flags_1E >> 2) & 1) && (action->flags & 2)) self->flags_E4 |= 0x80;
                return 2;
            }
        }
        {
            s32 can_act = 0;
            if (D_80148090 == 0) can_act = func_800E1CC4(self, 0) == 0;
            if (can_act) {
                mode = 0;
                if (action->flags & 4) {
                    if (action->flags & 2) mode = 2;
                    else mode = 3;
                } else if ((action->flags & 2) && controls_enabled(&D_80142F18)) mode = 1;
                if (mode != 0) {
                    func_800D72E0((u8 *)self, mode);
                    return 1;
                }
            }
        }
        if (func_800A4CC4(self, func_800A7DEC(self), &direction)) {
            if (func_800FCF3C(&next, 0)) return 0;
            func_800A4EC0(self, &direction);
            return 2;
        }
    }
    func_8006D438();
    return 1;
}
