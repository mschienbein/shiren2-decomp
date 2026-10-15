#include "map_rect.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800B3774(MapPos *out);
s32 func_800A251C(void *x, MapPos *y);
s32 func_800F6264(u8 *obj) {
    MapPos tmp;
    func_800B3774(&tmp);
    return func_800A251C(obj + 0x8C, &tmp);
}
