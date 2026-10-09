#include "common.h"
typedef struct { s32 x; s32 y; } Point;
extern void *func_800B4928(Point *);
s32 func_80041638(s32 y, s32 x) { Point point; point.y = y; point.x = x; return func_800B4928(&point) != 0; }
