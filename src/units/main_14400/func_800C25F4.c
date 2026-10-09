#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { s32 x0, y0, x1, y1; } Rect;
typedef struct Iterator Iterator;
extern s32 func_800A3138(Rect *span);
extern s32 func_800A315C(Rect *range);
extern void func_800C25D0(Iterator *obj, Pair *pair, u8 *byte, s32 value);

static inline Pair *set_pair(Pair *pair, s32 x, s32 y) {
    pair->x = x;
    pair->y = y;
    return pair;
}

static inline u8 *set_direction(u8 *direction, u8 value) {
    *direction = value;
    return direction;
}

void func_800C25F4(Iterator *iter, Rect *rect, s32 side, s32 extend) {
    Pair position;
    u8 direction0, direction1, direction2, direction3;
    s32 x0 = rect->x0;
    s32 y0 = rect->y0;
    s32 y1 = rect->y1;
    s32 x1 = rect->x1;
    s32 height = func_800A3138(rect);
    s32 width = func_800A315C(rect);
    switch (side) {
    case 0:
        if (extend) { x0--; width += 2; }
        func_800C25D0(iter, set_pair(&position, x0, y1 + 1), set_direction(&direction0, 6), width);
        break;
    case 1:
        func_800C25D0(iter, set_pair(&position, x0 - 1, y0), set_direction(&direction1, 0), height);
        break;
    case 2:
        if (extend) { x0--; width += 2; }
        func_800C25D0(iter, set_pair(&position, x0, y0 - 1), set_direction(&direction2, 6), width);
        break;
    default:
        func_800C25D0(iter, set_pair(&position, x1 + 1, y0), set_direction(&direction3, 0), height);
        break;
    }
}
