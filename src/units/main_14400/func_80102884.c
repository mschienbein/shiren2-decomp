#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Position;

/* Partial view of the searching object: its map position is the first member. */
typedef struct {
    Position pos; /* 0x00 */
} Object;

extern u8 D_80147620[];
Position func_800A6B70(u8 *src, u8 mode, s32 arg);
s32 func_800A692C(void *object, s32 code);
s32 func_80102A78(void *object);
void *func_800B36C4(void *out, Position *pos, u8 kind, s32 arg3);
u8 func_800C57A0(void *rng);

/* Picks a destination for obj into *out and returns out. */
Position *func_80102884(Position *out, Object *obj) {
    Position start;
    Position found;
    Position fallback;
    Position result;
    Position *p = &start;

    p->x = obj->pos.x;
    p->y = obj->pos.y;
    result = func_800A6B70((u8 *)obj, 2, 0);
    fallback = result;
    if (func_800A692C(obj, 0x15)) {
        found.x = 0;
        found.y = 0;
    } else {
        func_800B36C4(&result, p, 0x10, func_80102A78(obj));
        found = result;
    }
    if ((found.y | found.x) == 0) {
        out->x = fallback.x;
        out->y = fallback.y;
    } else if ((fallback.y | fallback.x) == 0 || !(func_800C57A0(D_80147620) & 1)) {
        out->x = found.x;
        out->y = found.y;
    } else {
        out->x = fallback.x;
        out->y = fallback.y;
    }
    return out;
}
