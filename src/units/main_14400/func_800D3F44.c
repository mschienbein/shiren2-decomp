#include "common.h"
#include "map_position_iterator.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 value; } Dir;
typedef struct { MapRect bounds; u8 side_counts[4]; } RoomArea800D3F44;
/* D459C initializes the complete 16-byte state before forwarding it via D3EF0. */
typedef struct { RoomArea800D3F44 *area; s32 kind; s32 field_08; s32 field_0C; } RoomState800D3F44;
void *func_800A2594(void *out, void *from, Dir dir);
void func_800B1B58(MapPos *pos, u16 flags);
static __inline__ void copyPt(MapPos *dst, MapPos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static __inline__ s32 rangeActive(MapPositionIterator *r) { return r->cur.x <= r->end.x; }
void func_800D3F44(RoomState800D3F44 *arg0) {
    MapPositionIterator range;
    MapPos first;
    MapPos srcA;
    MapPos last;
    MapPos srcB;
    Dir modes[2];

    copyPt(&srcA, &arg0->area->bounds.min);
    modes[0].value = 7;
    func_800A2594(&first, &srcA, modes[0]);
    copyPt(&srcB, &arg0->area->bounds.max);
    modes[1].value = 3;
    func_800A2594(&last, &srcB, modes[1]);
    range.start = first;
    range.cur = range.start;
    range.end = last;
    while (rangeActive(&range)) {
        func_800A3610(&first, &range);
        func_800B1B58(&first, 0x2000);
    }
}