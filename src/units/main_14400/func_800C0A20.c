#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x3DC]; s32 x3DC; } Self;
/* Runtime room: the rectangle is followed by four per-side exit counts.
 * Keep the full 0x14-byte record; this declaration does not allocate the table. */
typedef struct { MapRect bounds; u8 sides[4]; } RoomRecord;
extern MapRect D_801429C0;
extern RoomRecord D_801431F0[];
extern u8 D_8014344C;
extern u8 D_80143392;
void func_800B1BE0(MapPos *v, s32 arg);
void func_800B1B58(MapPos *v, u16 flags);
void func_800B17A4(void);

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
void func_800C0A20(Self *self) {
    MapPositionIterator it;
    MapPos tmp;
    MapPositionIterator *iter = &it;
    MapPos *cur = &tmp;
    iter_set_start(&it, &tmp, D_801429C0.min.x, D_801429C0.min.y);
    iter_set_end(&it, &tmp, D_801429C0.max.x, D_801429C0.max.y);
    for (;;) {
        s32 valid = !(it.cur.x > iter->end.x);
        if (!valid) {
            break;
        }
        func_800A3610(cur, iter);
        func_800B1BE0(cur, 0x4000);
        func_800B1B58(cur, 0x1280);
    }

    func_800B6728(D_801431F0, &D_801429C0);
    self->x3DC = 1;
    D_8014344C = 1;
    func_800B17A4();
    D_80143392 = 5;
}
