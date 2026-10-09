#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { void *source; s32 type, auxiliary; short amount; u16 options; u8 scale; } Message;
typedef struct { u8 pad_00[0x68]; short adjustment_68, unused_6A; u32 (*power_6C)(void *); } Table;
typedef struct { Pos position; Dir direction_08; u8 pad_09[0x13]; u16 flags_1C; u8 pad_1E[6]; Table *vtable_24; } Actor;
extern s32 func_800E1CC4(void *, s32);
extern s32 func_800A4754(void *, void *, Dir *);
extern void func_800A2758(void *, Dir);
extern void *func_800B4928(Pos *);
extern s32 func_800A674C(void *, void *);
extern u32 func_800B1C6C(Pos *);
extern s32 func_800E33D0(void *, Message, u8, u8);
static inline void copy_position(Pos *out, Pos *in) { out->x = in->x; out->y = in->y; }
static inline Dir read_direction(Actor *actor) { return actor->direction_08; }
static inline s32 cardinal(Dir *direction) { return (direction->value ^ 1) & 1; }
static inline Message *make_message(Message *message, u32 power) {
    message->amount = power;
    message->options = 0;
    message->scale = 10;
    message->auxiliary = 0;
    return message;
}
/* Actor vtable slot 0xB4 override (D_8015B440+0xB4). Slot callers (func_80100EE8 at
 * 0x80100F04, func_800F27A4 at 0x800F2B28) pass the receiver and a target in a1; this
 * override never reads the target. */
s32 func_80100F24(Actor *self, void *slot_target) {
    Pos position;
    Message message;
    Message copy;
    Dir direction;
    s32 limit;
    s32 steps;
    copy_position(&position, &self->position);
    direction = read_direction(self);
    {
        s32 reduced = func_800E1CC4(self, 2);
        limit = 3;
        if (reduced) limit = 1;
    }
    steps = 0;
    for (;;) {
        s32 more = steps++ < limit;
        s32 failed;
        s32 blocked;
        Actor *target;
        if (!more) break;
        failed = 0;
        if (!cardinal(&direction)) failed = !func_800A4754(self, &position, &direction);
        if (failed) { steps--; break; }
        func_800A2758(&position, direction);
        blocked = 0;
        target = func_800B4928(&position);
        if (target && func_800A674C(self, target)) {
            s32 inactive = target->flags_1C & 1;
            blocked = !inactive;
        }
        if (blocked) break;
        if (func_800B1C6C(&position) & 0x4000) { steps--; break; }
    }
    copy = *make_message(&message, self->vtable_24->power_6C((u8 *)self + self->vtable_24->adjustment_68));
    func_800E33D0(self, copy, 1, steps);
    return 1;
}
