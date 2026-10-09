#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { u8 value; } Direction;
typedef struct { short delta, index; void *fn; } Method;
typedef struct Object Object;
struct Object {
    Position position; u8 direction, terrain, id; u8 reserved_0B[0x11];
    u16 flags_1C; u8 flags_1E, reserved_1F; u32 flags_20; Method *table;
    u8 reserved_28[0x2C]; u8 state_54; u8 reserved_55[3]; Object *target;
    u8 reserved_5C[0x16]; u8 flags_72; u8 reserved_73[3]; u8 stuck_76;
    u8 reserved_77[0x11]; u8 chance_88; u8 reserved_89[3]; void *inventory_8C;
    u8 reserved_90[0xA]; u16 flags_9A; u8 reserved_9C, level_9D, chance_9E, priority_9F;
};
typedef struct { s32 value; Method *table; } Embedded;
typedef struct { u8 kind, id, flags; u8 reserved_03[9]; Embedded state_0C; } Item;
typedef struct { Object *source; s32 kind, field_08; short amount; u16 flags; } Damage;
typedef union { Damage *damage; s32 number; } EventValue;
typedef union { Direction *direction; s32 number; } EventExtra;
typedef struct { u32 kind; Object *target; u8 reserved_08[8]; EventValue value; EventExtra extra; } Event;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern u32 D_8013960C;
extern u8 D_80143094[], D_80147620[];
extern Object *D_801476B8;
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...), func_800A08D8(s32, s32, s32);
extern Direction *func_800A22B8(Direction *, void *, void *);
extern char *func_800A3B20(Object *);
extern s32 func_800A44F4(void *, void *), func_800A46BC(Object *, Position *, u8 *), func_800A4754(Object *, Position *, u8 *);
extern s32 func_800A50AC(Object *), func_800A50E8(Object *), func_800A58B8(Object *), func_800A65B8(Object *, Object *);
extern u8 func_800A6420(Object *, Object *);
extern void *func_800A65E4(u8 *, Object *, Object *);
extern void func_800A665C(Object *, u8 *);
extern Object *func_800A6CF0(Object *);
extern Item *func_800AACE0(s32);
extern s32 func_800ADC90(Item *, void *, void *), func_800AFDBC(void *), func_800B502C(Object *);
extern u32 func_800B1C6C(Position *);
extern u8 func_800C57A0(void *);
extern s32 func_800C587C(void *, u8), func_800CD090(void *, void *), func_800CF47C(void *), func_800D78B4(Object *);
extern u16 func_800E08B0(Object *);
extern s32 func_800E1CC4(Object *, s32), func_800E1CD4(Object *, s32), func_800E1D14(Object *, s32), func_800E1D54(Object *), func_800E20CC(Object *);
extern s32 func_800E3D20(Object *, Damage *), func_800E4B60(Object *, Event *), func_800E66EC(Object *);
extern void func_800E42AC(Object *, Damage *);
extern s32 func_800E776C(Object *, u8), func_800E8350(Object *), func_800E8694(Object *);
extern Item *func_800F0314(Object *);
extern s32 func_800F0404(Object *), func_800F0440(Object *), func_800F0C4C(Object *);
extern void func_800F06E4(Object *), func_800F073C(Object *), func_800F0A08(Object *), func_800F0CB0(Object *, Object *);
extern s32 func_800F0EC4(Object *), func_800F0F84(Object *, u8);
extern void func_800F16C0(Object *, s32), func_800F3408(Object *, Event *);
extern s32 func_800F3310(Object *), func_800F3538(Object *, Object *, s32, Direction), func_800F3718(Object *, Object *);
extern s32 func_800F38AC(Object *, u8, u8);
extern s32 func_80121940(Item *, Object *, Object *, s32);
extern Object *func_801E8DBC(void);

static inline s32 in_event_mode(void) { return D_80142F18.mode == 0x4F; }
static inline s32 holding(Object *self) { return ((u8)self->flags_9A & 0x80) != 0; }
static inline s32 released(Object *self) { return ((u8)self->flags_9A & 0x80) == 0; }
static inline s32 attack(Object *self, Object *target) {
    Method *method = &self->table[0xB0/8];
    return ((s32 (*)(void *, void *))method->fn)((u8 *)self + method->delta, target);
}
static inline s32 act(Object *self) {
    Method *method = &self->table[0xA8/8];
    return ((s32 (*)(void *))method->fn)((u8 *)self + method->delta);
}
static inline void *inventory(Object *self) {
    Method *method = &self->table[0x98/8];
    return ((void *(*)(void *))method->fn)((u8 *)self + method->delta);
}
static inline void change_status(Object *self, s32 mode, s32 kind, u8 value, s32 extra) {
    Method *method = &self->table[0x90/8];
    ((s32 (*)(void *, s32, s32, u8, s32))method->fn)((u8 *)self + method->delta, mode, kind, value, extra);
}

/* Hands the carried item over (flag 0x80 of +0x9A); reports why when it cannot. */
static inline void hand_over(Object *self) {
    Item *item = func_800F0314(self);
    char *name = func_800A3B20(self);
    s32 message;
    if (!item) message = 0xAD;
    else {
        if (!func_800AFDBC(D_80143094)) message = 0xAC;
        else if (func_800CD090(inventory(D_801476B8), item) != -1) {
            if (func_80121940(item, self, D_801476B8, 0)) {
                self->flags_9A &= ~0x80;
                return;
            } else {
                Embedded *state = &item->state_0C;
                Method *method = &state->table[0x10/8];
                if (((s32 (*)(void *))method->fn)((u8 *)state + method->delta)) return;
                message = 0xAB;
            }
        } else message = 0xAA;
    }
    self->flags_9A &= ~0x80;
    func_800498E4(message, name);
}

s32 func_800F27A4(Object *self, Event *event) {
    Position position;
    u8 direction0, direction1, direction2;
    Direction direction3;
    u8 direction4;
    position.x = self->position.x;
    position.y = self->position.y;
    switch (event->kind) {
    case 0: {
        Object *target;
        s32 water;
        if (in_event_mode()) {
            Object *leader = func_801E8DBC();
            if (leader && !(func_800C57A0(D_80147620) & 3)) {
                if ((leader->id == 0x29 && leader == self) || (leader->id != 0x29 && leader->id == self->id)) {
                    self->target = 0;
                    self->state_54 |= 4;
                    return 0;
                }
            }
            if (func_800A50AC(self)) return 1;
            return func_800A50E8(self);
        }
        func_800F073C(self);
        target = self->target;
        water = 0;
        if ((self->terrain & 15) == 1) {
            if (func_800B1C6C(&position) & 0x80) water = 1;
        }
        if (water) {
            if (func_800A58B8(self) == 2 && target && target == func_800A6CF0(self)) self->state_54 |= 4;
            return 0;
        }
        if (func_800E8694(self)) {
            s32 wait = 0;
            if (!(self->flags_9A & 1) || !func_800E1CD4(self, 0x10)) wait = 1;
            if (wait) return func_800E8350(self);
        }
        if (self->flags_9A & 0x80) {
            s32 reachable = 0;
            if (func_800A65B8(self, D_801476B8) == 1) {
                func_800A65E4(&direction0, self, D_801476B8);
                reachable = func_800A4754(self, &position, &direction0) != 0;
            }
            if (reachable) { func_800F06E4(self); self->target = 0; return 0; }
            if (!func_800A6420(self, target)) { func_800F06E4(self); return 0; }
            if (func_800E776C(self, 8)) return 1;
            return func_800E66EC(self);
        }
        if (self->flags_9A & 0x100) return func_800E66EC(self);
        if (func_800F0EC4(self)) return func_800F3310(self);
        {
            Method *method = &self->table[0xA0/8];
            return ((s32 (*)(void *))method->fn)((u8 *)self + method->delta);
        }
    }
    case 25: {
        Object *target = event->target;
        Method *method = &self->table[0x40/8];
        s32 permitted;
        if (((s32 (*)(void *, void *, u8 *))method->fn)((u8 *)self + method->delta, target, &self->priority_9F) == 2) self->target = target;
        permitted = 0;
        if (func_800F0F84(self, 100) && ((self->flags_9A & 1) || !target || !func_800B502C(target))) permitted = 1;
        if (permitted) {
            func_800A65E4(&direction1, self, target);
            func_800A665C(self, &direction1);
            if (func_800E1CC4(self, 4)) { direction2 = (self->direction + 4) & 7; func_800A665C(self, &direction2); }
            if (attack(self, func_800A6CF0(self))) return 1;
            return act(self);
        }
    }
    /* An unhandled target request falls through to the ordinary action. */
    case 1: {
        Object *target;
        Object *ahead;
        s32 special;
        s32 use_attack;
        s32 use_action;
        if (in_event_mode()) {
            s32 idle;
            D_8013960C <<= 1;
            idle = 0;
            if ((func_800C57A0(D_80147620) & 1) || !attack(self, func_800A6CF0(self))) idle = 1;
            if (idle) func_80049CB4(0x47, self, 1, 0);
            D_8013960C >>= 1;
            return 1;
        }
        if (func_800E20CC(self)) self->target = 0;
        target = self->target;
        if (target) {
            Method *method = &target->table[0x10/8];
            if (((s32 (*)(void *))method->fn)((u8 *)target + method->delta)) return 1;
            func_800A22B8(&direction3, self, target);
            func_800A665C(self, &direction3.value);
        } else {
            special = 0;
            if (holding(self) && func_800A65B8(self, D_801476B8) == 1) special = func_800E20CC(self) == 0;
            if (special) {
                hand_over(self);
                if (released(self)) func_800A08D8(0, -1, 0);
                return 1;
            }
        }
        if (func_800E1CC4(self, 4)) { direction4 = (self->direction + 4) & 7; func_800A665C(self, &direction4); }
        ahead = func_800A6CF0(self);
        use_attack = 0;
        if (!func_800F0EC4(self)) use_attack = func_800E20CC(self) == 0;
        if (use_attack) {
            use_attack = 0;
            if (!target || (func_800A6420(self, target) && (self->flags_9A & 1)) || (func_800C587C(D_80147620, self->chance_88) && (target == ahead || (self->flags_9A & 1)))) use_attack = 1;
            if (use_attack && attack(self, ahead)) { self->stuck_76 = 0; return 1; }
        }
        use_action = 0;
        if (func_800E1CC4(self, 4) || func_800E1D14(self, 0x13) || func_800E1CC4(self, 0) || func_800E20CC(self) || (ahead && func_800A46BC(self, &position, &self->direction) && func_800A44F4(self, ahead) != 1)) use_action = 1;
        if (use_action) { self->stuck_76 = 0; return act(self); }
        return 0;
    }
    case 2: {
        s32 alter;
        if (self->flags_72 & 1) func_800F0A08(self);
        alter = 0;
        if (func_800E1D54(self)) alter = (self->terrain & 15) == 2;
        if (alter) { change_status(self, 1, 13, 0, 0); change_status(self, 0, 13, 0xFE, 0); }
        break;
    }
    case 5: {
        Item *item;
        if ((event->value.number == 4 || event->value.number == 5) && (item = func_800F0314(self)) && func_800CD090(inventory(D_801476B8), item) != -1) func_800F0404(self);
        else {
            s32 halve = 0;
            if (self->flags_9A & 0x40) {
                if (self->flags_9A & 0x200) halve = 1;
            }
            if (halve) { self->level_9D >>= 1; func_800D78B4(self); }
        }
        break;
    }
    case 8:
        if (in_event_mode()) return 1;
        func_800F0CB0(self, event->value.damage->source);
        break;
    case 9:
        if (!in_event_mode()) {
            Damage *damage = event->value.damage;
            s32 retaliate;
            func_800F0CB0(self, damage->source);
            func_800F3408(self, event);
            retaliate = 0;
            if (self->flags_9A & 8) {
                if (!(self->flags_9A & 0x40) && func_800E08B0(self)) retaliate = func_800C587C(D_80147620, self->chance_9E) != 0;
            }
            if (retaliate && damage->amount > 0 && damage->kind != 11 && damage->kind != 31) func_800F0C4C(self);
        }
        return 1;
    case 10:
        if (!in_event_mode()) {
            Damage *damage = event->value.damage;
            if (func_800E3D20(self, damage)) func_800F16C0(self, damage->kind == 0x27);
            if (damage->kind != 0x1E) {
                s32 halve;
                D_8013960C = (D_8013960C << 1) | 1;
                func_800F0440(self);
                D_8013960C >>= 1;
                halve = 0;
                if (self->flags_9A & 0x40) {
                    if (self->flags_9A & 0x200) halve = 1;
                }
                if (halve) { self->level_9D >>= 1; func_800D78B4(self); }
            }
            func_800E42AC(self, damage);
        }
        return 1;
    case 12: {
        s32 empty = 0;
        if (!self->inventory_8C || !func_800CF47C(self->inventory_8C)) empty = 1;
        if (empty && (self->flags_72 & 8)) {
            Item *item = func_800AACE0(0);
            if (item) { item->flags |= 0x40; func_800ADC90(item, self, self); }
        }
        self->flags_72 &= ~8;
        return 1;
    }
    case 13: return func_800F3718(self, event->target);
    case 17: self->target = event->target; self->priority_9F = 10; return 1;
    case 19: func_800F0CB0(self, event->target); return func_800F3538(self, event->target, event->value.number, *event->extra.direction);
    case 21: return func_800F38AC(self, event->value.number, event->extra.number);
    }
    return func_800E4B60(self, event);
}
