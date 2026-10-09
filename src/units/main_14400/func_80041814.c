#include "common.h"
typedef struct { s32 x, y; } Position;
typedef struct { Position from, to; } Area;
extern s32 D_80138C40[4];
s32 func_800464A4(void *self, void *grid, Area *area);
void func_80041814(void *grid, s32 y1, s32 x1, s32 y2, s32 x2) {
    Area area;
    Area source;
    source.from.y = y1;
    source.from.x = x1;
    source.to.y = y2;
    source.to.x = x2;
    area.from = source.from;
    area.to = source.to;
    func_800464A4(&D_80138C40, grid, &area);
}
