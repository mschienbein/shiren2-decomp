#include "common.h"
typedef unsigned char u8;
typedef struct ShirenDirection { signed char value; } ShirenDirection;
typedef struct { s32 x, y; } Position;
typedef struct { Position position; u8 direction; } Unit;
typedef struct { u8 state[0x18]; } Iterator;
extern void func_800C27D0(void *iterator, void *origin, ShirenDirection direction, u8 limit, u8 mode);
extern void *func_800C28FC(void *out, void *iterator);
extern int func_800C28EC(const u8 *iterator);
extern void *func_800B4928(Position *position);
extern s32 func_800A44F4(void *self, void *target);
extern Position *func_800A25D8(Position *out, Position *position, ShirenDirection direction, s32 scale);
extern void func_8010E70C(void *self, void *unit, void *direction, void *origin, void *position);

static __inline__ void copy_position(Position *out, Position *in) { out->x = in->x; out->y = in->y; }
static __inline__ void copy_direction(ShirenDirection *out, Unit *unit) { out->value = unit->direction; }
/* Line iterator over up to three cells from the unit along its facing. */
static __inline__ void iterator_init(Iterator *it, Unit *unit, ShirenDirection direction) {
    func_800C27D0(it, unit, direction, 3, 0);
}

void func_80123490(void *self, Unit *unit) {
    Position origin, destination;
    Iterator iterator;
    Position current;
    ShirenDirection direction;
    void *target = 0;

    copy_direction(&direction, unit);
    copy_position(&origin, &unit->position);
    iterator_init(&iterator, unit, direction);
    func_800C28FC(&current, &iterator);
    while (func_800C28EC((const u8 *)&iterator)) {
        func_800C28FC(&current, &iterator);
        destination = current;
        target = func_800B4928(&destination);
        if (target != 0 && func_800A44F4(unit, target) == 2) break;
        target = 0;
    }
    if (target == 0) {
        func_800A25D8(&current, &origin, direction, 2);
        destination = current;
    }
    func_8010E70C(self, unit, &direction, &origin, &destination);
}
