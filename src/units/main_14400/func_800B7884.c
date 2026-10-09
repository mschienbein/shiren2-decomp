#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { Pair current, start, end; } Iter;
typedef struct { Pair start, end; } Rectangle;
extern Rectangle D_801429C0;
void func_800B1080(void);
Pair *func_800A3610(Pair *out, Iter *iterator);
void func_800B1AE0(Pair *position, unsigned short flags);
static inline s32 first_x(Rectangle *r) { return r->start.x; }
static inline s32 first_y(Rectangle *r) { return r->start.y; }
static inline s32 last_x(Rectangle *r) { return r->end.x; }
static inline s32 last_y(Rectangle *r) { return r->end.y; }
void func_800B7884(void)
{
    Iter iterator;
    Iter *cursor;
    Pair position;
    func_800B1080();
    cursor = &iterator;
    position.x = first_x(&D_801429C0);
    position.y = first_y(&D_801429C0);
    iterator.start = position;
    iterator.current = iterator.start;
    position.x = last_x(&D_801429C0);
    position.y = last_y(&D_801429C0);
    iterator.end = position;
    for (;;) {
        s32 valid = iterator.current.x <= cursor->end.x;
        if (!valid) {
            break;
        }
        func_800A3610(&position, cursor);
        func_800B1AE0(&position, 0x4000);
    }
}
