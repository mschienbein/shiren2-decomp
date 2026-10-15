#include "map_rect.h"
extern s32 D_80138C40[4];
s32 func_800464A4(void *self, u32 (*grid)[54], MapRect *area);
void func_80041814(void *grid, s32 y1, s32 x1, s32 y2, s32 x2) {
    MapRect area;
    MapRect source;
    source.min.y = y1;
    source.min.x = x1;
    source.max.y = y2;
    source.max.x = x2;
    area.min = source.min;
    area.max = source.max;
    func_800464A4(&D_80138C40, grid, &area);
}
