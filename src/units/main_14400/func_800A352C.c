#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;


/* Three consecutive 16-byte rectangles; D_801429B8 and D_801429D8 are their +8 members. */
extern MapRect D_801429B0;
extern MapRect D_801429C0;
extern MapRect D_801429D0;

static inline void pair_copy(MapPos *dst, MapPos *src) {
    *dst = *src;
}
void func_800A352C(void) {
    MapRect rect;
    MapPos *size;

    rect.min.x = 0;
    rect.min.y = 0;
    rect.max.x = 0;
    rect.max.y = 0;
    pair_copy(&D_801429B0.min, &rect.min);
    pair_copy(&D_801429B0.max, &rect.max);
    size = &rect.max;
    rect.min.x = 10;
    rect.min.y = 10;
    size->x = 0x2B;
    size->y = 0x41;
    pair_copy(&D_801429C0.min, &rect.min);
    pair_copy(&D_801429C0.max, &rect.max);
    size->x = 0x35;
    rect.min.x = 0;
    rect.min.y = 0;
    size->y = 0x4B;
    pair_copy(&D_801429D0.min, &rect.min);
    pair_copy(&D_801429D0.max, &rect.max);
}
