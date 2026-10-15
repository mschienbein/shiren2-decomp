#include "common.h"
#include "map_line_iterator.h"

typedef signed char s8;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
extern u32 func_800B1C6C(void *);
s32 func_800BB474(void *self, const MapPos *key, ShirenDirection kind, s32 limit) {
    MapLineIterator it;
    MapPos item;
    MapLineIterator *iter = &it;
    unsigned char n;
    /* Read the representation of the actual signed direction byte. */
    func_800C25D0(iter, key, (const unsigned char *)&kind.value, limit);
    n = 0;
    while (1) {
        if (iter->index >= iter->length) break;
        func_800C2758(&item, iter);
        if (func_800B1C6C(&item) & 0x800) break;
        n++;
    }
    return n ? n - 1 : 0;
}
