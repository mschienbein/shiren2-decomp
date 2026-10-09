#include "common.h"

typedef struct { s32 x, y; } Point;
extern unsigned char D_80145460[];

s32 func_800B4F74(Point *point) {
    s32 y = point->y;
    s32 x;
    s32 outside = 0;
    if (y >= 0x4C || (x = point->x, x >= 0x36) || y < 0 || x < 0) {
        outside = 1;
    }
    if (outside != 0) {
        return 0;
    }
    return D_80145460[point->y + point->x * 0x4C] != 0xFF;
}
