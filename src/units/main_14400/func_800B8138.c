#include "common.h"
#include "map_rect.h"
typedef struct { unsigned char v; } Dir;
extern u32 func_800B1C6C(MapPos *position);
extern s32 func_800B43BC(void *position, s32 arg1, unsigned char arg2);
extern s32 func_800B38F8(MapPos *position);
extern void func_800A2758(MapPos *position, Dir direction);
/* The map owner is supplied by callers but this operation only updates the position. */
void func_800B8138(void *object, MapPos *position, unsigned char *direction, s32 count) {
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
