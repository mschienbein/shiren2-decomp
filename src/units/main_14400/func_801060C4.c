#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { Position position; char pad8[0x50]; void *target; char pad5C[0x3E]; u16 flags_9A; } Unit;

u32 func_800B1C6C(void *pos);
u8 func_800A6420(void *obj, void *target);
void *func_800A65E4(void *out_direction, void *obj, void *target);
void func_800A665C(void *obj, u8 *value);
s32 func_800A67DC(void *unit, void *target, s32 force, s32 apply);
s32 func_800E65C0(void *unit, void *target, s32 mode);
s32 func_800E7AA8(void *obj, s32 a1);
void func_800F06E4(void *obj);
s32 func_800F1024(void *unit);
s32 func_800A50AC(void *object);

static inline Position *copy_position(Position *dest, Position *source) {
    dest->x = source->x;
    dest->y = source->y;
    return dest;
}

static inline u16 has_mask(u32 bits, u32 mask) { return (bits & mask) != 0; }

static inline void face(Unit *self, void *target) {
    u8 direction;
    func_800A65E4(&direction, self, target);
    func_800A665C(self, &direction);
}

s32 func_801060C4(Unit *self) {
    Position position;
    s32 active = has_mask(func_800B1C6C(copy_position(&position, &self->position)), 0x2000);
    void *target = self->target;
    void *goal = self->target;
    u8 mode = func_800A6420(self, target);
    switch (mode) {
    case 0:
        func_800F06E4(self);
        return 0;
    case 1:
    case 2:
        if (active) {
            s32 apply = 0;
            if (self->flags_9A & 0x40) apply = 1;
            if (func_800A67DC(self, target, 0, apply)) {
                if (func_800F1024(self)) func_800F06E4(self);
                face(self, target);
                return 0;
            }
            if (!func_800E65C0(self, target, 0)) return func_800E7AA8(self, 0x4C);
            return 1;
        }
        face(self, goal);
        return 0;
    default:
        if (active) return func_800A50AC(self);
        return 0;
    }
}
