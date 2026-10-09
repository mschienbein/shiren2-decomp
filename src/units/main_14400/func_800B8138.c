#include "common.h"
typedef struct { s32 x; s32 y; } Point;
typedef struct { unsigned char v; } Dir;
extern u32 func_800B1C6C(Point *position);
extern s32 func_800B43BC(void *position, s32 arg1, unsigned char arg2);
extern s32 func_800B38F8(Point *position);
extern void func_800A2758(Point *position, Dir direction);
/* The map owner is supplied by callers but this operation only updates the position. */
void func_800B8138(void *object, Point *position, unsigned char *direction, s32 count) {
    for (;;) {
        Dir step;
        if (count-- <= 0) break;
        if (func_800B1C6C(position) & 0x8000) break;
        func_800B43BC(position, 0, 0);
        func_800B38F8(position);
        step.v = *direction;
        func_800A2758(position, step);
    }
}
