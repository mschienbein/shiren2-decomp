#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u8 D_80143391;
extern MapRect D_801429C0;
extern u16 D_80143450[][0x4C];
extern u8 D_80143448;
/* Two 0x18-byte region records initialized by func_800B1080: owner byte +0 (func_800D1D90
 * clears it with sb zero,0(a0)) plus 3 padding bytes, area pointer +4; rest not interpreted. */
typedef struct { signed char owner; u8 pad1[3]; void *area; u8 pad8[0x10]; } Region;
extern Region D_80143330[2];
void func_800D3648(void *arg0, s32 arg1);
void func_800D3D48(void);
static inline void iter_set_start(MapPositionIterator *it, MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    it->start = *p;
    it->cur = it->start;
}
static inline void iter_set_end(MapPositionIterator *it, MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    it->end = *p;
}
void func_800B261C(void) {
    MapPositionIterator it;
    MapPositionIterator *p;
    MapPos pos;
    D_80143391 |= 8;
    p = &it;
    iter_set_start(&it, &pos, D_801429C0.min.x, D_801429C0.min.y);
    iter_set_end(&it, &pos, D_801429C0.max.x, D_801429C0.max.y);
    for (;;) {
        s32 more = it.cur.x <= p->end.x;
        if (!more) {
            break;
        }
        func_800A3610(&pos, p);
        D_80143450[pos.x][pos.y] &= ~0x10;
    }
    if (D_80143448 != 0) {
        func_800D3648(&D_80143330[0], 0);
        func_800D3D48();
        D_80143448 = 1;
    }
}
