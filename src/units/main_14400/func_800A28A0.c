#include "common.h"
#include "map_rect.h"


void func_800A28A0(MapPos *record)
{
    s32 original00 = record->x;
    s32 original04 = record->y;

    record->y = original00;
    record->x = original04;
}
