#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Position;
typedef struct { u8 value; } Dir;
/* Partial unit view: map position at +0. */
typedef struct { Position pos; } Unit;

s32 func_800E6364(Unit *unit, Position *from, Position *to, s32 mode);
void func_800A2758(Position *pos, Dir dir);
s32 func_800A251C(Position *pos, Position *target);

static inline s32 previous(s32 count) {
    return count - 1;
}

static inline Dir *setDir(Dir *dir, s32 step) {
    dir->value = step & 7;
    return dir;
}

s32 func_800E62BC(Unit *unit, Position *target, s32 range) {
    Position pos;
    Dir dir;
    s32 step;

    pos.x = unit->pos.x;
    pos.y = unit->pos.y;
    while ((range = previous(range)) != -1) {
        step = func_800E6364(unit, &pos, target, 1);
        if (step == -1) {
            continue;
        }
        func_800A2758(&pos, *setDir(&dir, step));
        if (func_800A251C(&pos, target)) {
            return 1;
        }
    }
    return 0;
}
