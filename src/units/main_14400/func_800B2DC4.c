#include "map_rect.h"

extern void func_800B2DEC(MapRect *rect);

void func_800B2DC4(void *object)
{
    MapRect rect = func_800B3024(object);
    func_800B2DEC(&rect);
}
