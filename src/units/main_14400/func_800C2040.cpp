#include "common.h"
#include "map_rect.h"
#include "map_position_iterator.h"
/* Room rectangle followed by the four per-side exit counts: 20 bytes total. */
typedef struct { MapRect rect; unsigned char exits[4]; } Region_800C2040;
typedef struct { unsigned char field_0[0x3DC]; s32 field_3DC; unsigned char field_3E0[0x1C]; unsigned char field_3FC; } Object_800C2040;
extern "C" {
extern unsigned char D_80147620[], D_8014344C, D_80143392;
extern Region_800C2040 D_801431F0[];
extern s32 func_800C5844(void *rng, unsigned char base, unsigned char top);
extern void func_800B1B58(MapPos *, unsigned short);
extern void func_800B834C(void *center, s32 radius, unsigned short color);
extern void func_800B8138(Object_800C2040 *, MapPos *, unsigned char *, s32), func_800B17A4(void);
extern unsigned char func_800C57A0(void *rng);
}
static inline void position(MapPos *out, MapPos *point) { out->x = point->x; out->y = point->y; }
extern "C" void func_800C2040(Object_800C2040 *arg)
{
    /* Native construction belongs to these two arrays, not detached C loops. */
    MapRect rects[2];
    MapPos centers[2];
    MapPos low, high;
    MapPositionIterator iterator;
    MapPos point;
    unsigned char direction;
    s32 i;
    i = 0;
    centers[0].y = 0x18;
    centers[1].y = 0x34;
    centers[1].x = 0x1B;
    centers[0].x = 0x1B;
    for (;;) {
        s32 more = i < 2;
        s32 radius;
        unsigned short tile;
        MapPos *center;
        if (!more) break;
        center = &centers[i];
        radius = (unsigned char)func_800C5844(D_80147620, 5, 10);
        low.y = center->y - radius + 1;
        low.x = center->x - radius + 1;
        high.y = center->y + radius - 1;
        high.x = center->x + radius - 1;
        rects[i].min = low;
        rects[i].max = high;
        position(&point, &rects[i].min);
        iterator.start = point;
        iterator.cur = iterator.start;
        position(&point, &rects[i].max);
        iterator.end = point;
        tile = (i & 15) | 0x1000;
        for (;;) {
            s32 more = iterator.cur.x <= iterator.end.x;
            if (!more) break;
            func_800A3610(&point, &iterator);
            func_800B1B58(&point, tile);
        }
        func_800B834C(center, radius, (i & 15) | 0x1200);
        func_800B6728(&D_801431F0[i], &rects[i]);
        i++;
    }
    position(&low, &centers[0]);
    direction = 0;
    func_800B8138(arg, &low, &direction, centers[1].y - centers[0].y + 1);
    arg->field_3DC = 2;
    D_8014344C = 2;
    func_800B17A4();
    D_80143392 = 0;
    arg->field_3FC = func_800C57A0(D_80147620) & 1;
}
