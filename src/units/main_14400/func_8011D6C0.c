#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { u8 value; } Direction;
typedef struct { Position position; Direction direction; } Actor;
typedef struct {
    s32 kind; Actor *actor; u8 field_08[4]; Direction direction; u8 field_0D[3];
    Position position; s32 flags; u16 field_1C; u16 field_1E;
} Event;
extern s32 func_800A6EE0(void *object);
extern void func_8011D7A8(void *object, Actor *actor, Position *position, Direction direction, u16 flags);
extern s32 func_8011276C(void *object, Event *event);
static inline Position *copy_position(Position *out, Position *in) {
    out->x = in->x;
    out->y = in->y;
    return out;
}
s32 func_8011D6C0(void *object, Event *event) {
    Position position;
    switch (event->kind) {
    case 9: {
        Actor *actor = event->actor;
        Direction direction;
        s32 flags;
        Position *where = copy_position(&position, &actor->position);
        direction = actor->direction;
        flags = (func_800A6EE0(actor) | event->flags) & 0xFFFF;
        func_8011D7A8(object, actor, where, direction, flags);
        return 1;
    }
    case 17: {
        Position *where = copy_position(&position, &event->position);
        Actor *actor = event->actor;
        Direction direction = event->direction;
        func_8011D7A8(object, actor, where, direction, event->field_1E);
        return 1;
    }
    default:
        return func_8011276C(object, event);
    }
}
