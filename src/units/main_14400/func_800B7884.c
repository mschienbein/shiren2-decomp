#include "common.h"
#include "map_position_iterator.h"
extern MapRect D_801429C0;
void func_800B1080(void);
void func_800B1AE0(MapPos *position, unsigned short flags);
static inline s32 first_x(MapRect *r) { return r->min.x; }
static inline s32 first_y(MapRect *r) { return r->min.y; }
static inline s32 last_x(MapRect *r) { return r->max.x; }
static inline s32 last_y(MapRect *r) { return r->max.y; }
/* Floor-generator member: callers pass the generator (ROM 0x800BA9AC sets a0 in the
 * jal delay slot of func_800BA854's second call); this body does not use it. */
void func_800B7884(void *generator)
{
    MapPositionIterator iterator;
    MapPositionIterator *cursor;
    MapPos position;
    func_800B1080();
    cursor = &iterator;
    position.x = first_x(&D_801429C0);
    position.y = first_y(&D_801429C0);
    iterator.start = position;
    iterator.cur = iterator.start;
    position.x = last_x(&D_801429C0);
    position.y = last_y(&D_801429C0);
    iterator.end = position;
    for (;;) {
        s32 valid = iterator.cur.x <= cursor->end.x;
        if (!valid) {
            break;
        }
        func_800A3610(&position, cursor);
        func_800B1AE0(&position, 0x4000);
    }
}
