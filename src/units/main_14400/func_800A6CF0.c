#include "common.h"
typedef struct { s32 x; s32 y; } Point;
extern void *func_800A6CC0(void *out_position, void *obj);
extern void *func_800B4928(Point *pos);
void *func_800A6CF0(void *self) { Point point; func_800A6CC0(&point, self); return func_800B4928(&point); }
