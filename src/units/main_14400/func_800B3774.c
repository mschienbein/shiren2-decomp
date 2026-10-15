#include "map_rect.h"

/* Cached first special-tile position (zero until computed). */
extern MapPos D_80143360;

/* Returns the position by value (memory return through the hidden pointer). */
extern MapPos func_800B37E8(void);

static inline s32 read_x(MapPos *pair) { return pair->x; }
static inline s32 read_y(MapPos *pair) { return pair->y; }

void *func_800B3774(MapPos *out)
{
    if ((read_y(&D_80143360) | read_x(&D_80143360)) == 0) {
        MapPos fresh = func_800B37E8();
        D_80143360 = fresh;
    }
    out->x = read_x(&D_80143360);
    out->y = read_y(&D_80143360);
    return out;
}
