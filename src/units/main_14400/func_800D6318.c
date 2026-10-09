#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct { s32 x, y; } Point;

extern u8 D_80148000[];
s32 func_800A99D0(void);
s32 func_800D6784(Point *best, u8 *flag);
s32 func_800D6430(Point *best, u8 *flag);
s32 func_800D68EC(Point *out, s8 *flag);
s32 func_800D6D04(void *map, Point *pos, Point *best);

static inline s8 choose_point(s32 a, s32 b, Point *second, Point *first) {
    if (a) {
        if (b) {
            return func_800D6D04(D_80148000, second, first) != 0;
        } else {
            return 0;
        }
    } else {
        s32 result = -1;
        if (b) {
            result = 1;
        }
        return result;
    }
}

s32 func_800D6318(Point *out, u8 *tag) {
    s32 status[2];
    u8 tags[2];
    Point points[2];
    Point *second, *first;
    s32 i;
    s8 choice;

    for (i = 1; i != -1; --i) {
    }
    if (func_800A99D0()) {
        status[0] = func_800D6784(&points[0], &tags[0]);
    } else {
        status[0] = func_800D6430(&points[0], &tags[0]);
    }
    second = &points[1];
    first = &points[0];
    status[1] = func_800D68EC(second, (s8 *)&tags[1]);
    choice = choose_point(status[0], status[1], second, first);
    if (choice != -1) {
        *out = points[choice];
        *tag = tags[choice];
        return 1;
    }
    return 0;
}
