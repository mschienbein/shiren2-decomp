#include "map_ray_iterator.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* +0x20 is the u32 blocking mask stored by the constructor func_800C5150 (sw +0x20); this scan
   uses its low half, read as lhu +0x22. */
extern u32 func_800B1C6C(MapPos *pos);
extern void *func_800B4928(MapPos *pos);
/* ODD_C: Preserve the mask's halfword mode across the inline flag operation. */
static inline u16 blocked(u16 mask, MapPos *pos) {
    mask &= func_800B1C6C(pos);
    return mask;
}
void *func_800C51B8(MapRayIterator *iterator) {
    for (;;) {
        MapPos position;
        MapPos *pos;
        void *object;
        u16 mask;
        if (iterator->line.index >= iterator->line.length) break;
        func_800C2758(&position, &iterator->line);
        mask = (u16)iterator->mask;
        pos = &position;
        mask = blocked(mask, pos);
        if (mask) break;
        object = func_800B4928(pos);
        if (object) return object;
    }
    iterator->available = 0;
    return 0;
}
