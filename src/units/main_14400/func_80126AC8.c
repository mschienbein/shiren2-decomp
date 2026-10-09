#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Position;
typedef struct { Position position; u8 pad8[0x14]; unsigned short flags; } Actor;
typedef struct { u8 pad0[0x10]; Dir direction; } Object;
void *func_800A7DEC(void *object);
s32 func_800A4CC4(void *object, void *value, void *direction);
void func_800A59A4(Actor *actor);
void func_800A2758(Position *position, Dir direction);
void func_80115E18(void *object, void *position);
s32 func_80049CB4(s32 id, ...);
void func_800A58FC(void *actor, Position *position);
void func_800A7BA4(Actor *actor, s32 flags);
static inline Position *capture(Position *out, Position *position) {
    out->x = position->x;
    out->y = position->y;
    return out;
}
static inline s32 canMove(Actor *actor, Dir *direction) {
    return func_800A4CC4(actor, func_800A7DEC(actor), direction);
}
/* D_80160480+0x44: func_80115EB0 supplies seven pointer arguments at 0x80116028-0x80116050
 * and consumes the result at 0x80116054. arg1, arg3, arg4 and arg6 belong to that call
 * contract and are unused here. */
s32 func_80126AC8(Object *self, void *arg1, void *position, void *arg3, void *arg4, Actor *actor, void *arg6) {
    Position before, after;
    if (actor && !(actor->flags & 2) && canMove(actor, &self->direction)) {
        Position *previous = capture(&before, &actor->position);
        after.x = previous->x;
        after.y = previous->y;
        func_800A59A4(actor);
        func_800A2758(&after, self->direction);
        actor->position = after;
        func_80115E18(self, position);
        func_80049CB4(0x9C, actor, &before, &after);
        func_800A58FC(actor, &after);
        func_800A7BA4(actor, 3);
    }
    return 1;
}
