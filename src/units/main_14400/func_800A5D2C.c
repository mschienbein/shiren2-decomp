#include "map_ring_iterator.h"
extern s32 func_800A4314(void *object, MapPos *value);
/* flags is an int: func_800A5DC4 forwards its own a2 unmasked (0x800A5DD8); the body
 * narrows it to 16 bits for func_800C5280 (andi a2,a2,0xFFFF at 0x800A5D5C). */
s32 func_800A5D2C(void *object, MapPos *output, s32 flags) {
    MapRingIterator iterator;
    MapPos value;
    value.x = output->x;
    value.y = output->y;
    func_800C5280(&iterator, &value, flags);
    for (;;) {
        s32 active = func_800C559C(&iterator);
        if (!active) break;
        func_800C532C(&value, &iterator);
        if (func_800A4314(object, &value)) {
            *output = value;
            return 1;
        }
    }
    return 0;
}
