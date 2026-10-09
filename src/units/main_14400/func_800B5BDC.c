#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Point;
typedef struct { s32 x0, y0, x1, y1; } Rect;
/* Owner byte +0 (func_800D1D90 clears it with sb zero,0(a0)) plus 3 padding bytes, area
 * rectangle +4, remaining bytes not interpreted here. */
typedef struct { signed char owner_00; u8 pad_01[3]; Rect *bounds_04; u8 pad_08[0x10]; } Region;
/* 800B1080 initializes two consecutive 0x18-byte region objects. */
extern Region D_80143330[2];
extern u8 D_80143448;
extern u8 D_80143391;
extern s32 func_800A31C8(Rect *rectangle, Point *point);

s32 func_800B5BDC(Point *point)
{
    if (D_80143448 != 0) {
        s32 outside = 0;
        s32 i;
        Region *region;
        if (point->y >= 0x4C || point->x >= 0x36 || point->y < 0 || point->x < 0) {
            outside = 1;
        }
        if (outside) {
            return 0;
        }
        i = 0;
        region = D_80143330;
        for (;;) {
            if (i >= D_80143448) {
                return 0;
            }
            if (region->bounds_04 == 0) {
                u8 flag = D_80143391 & 4;
                return flag != 0;
            }
            if (func_800A31C8(region->bounds_04, point)) {
                return 1;
            }
            ++region;
            ++i;
        }
    }
    return 0;
}
