#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Point;

s32 func_800B1AB8(Point *p) {
    return (u32)(p->y - 10) < 0x38 && (u32)(p->x - 10) < 0x22;
}
