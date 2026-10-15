#include "map_ray_iterator.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct ShirenDirection {
    s8 value;
} ShirenDirection;


void *func_800C5150(MapRayIterator *it, MapPos *pos, ShirenDirection dir, s32 value, u32 limit) {
    /* Unsigned-character access to the signed direction byte's representation. */
    func_800C25D0(&it->line, pos, (const u8 *)&dir.value, value);
    it->origin = *pos;
    it->available = 1;
    it->mask = limit;
    return it;
}
