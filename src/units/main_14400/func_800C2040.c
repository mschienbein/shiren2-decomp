#include "common.h"
typedef struct { s32 x, y; } Point;
typedef struct { Point low, high; } Rect;
typedef struct { Point current, first, last; } Iterator;
typedef struct { s32 field_0[5]; } Region;
typedef struct { unsigned char field_0[0x3DC]; s32 field_3DC; unsigned char field_3E0[0x1C]; unsigned char field_3FC; } Object;
extern unsigned char D_80147620[], D_8014344C, D_80143392;
extern Region D_801431F0[];
extern s32 func_800C5844(void *rng, unsigned char base, unsigned char top);
extern void *func_800A3610(void *out, void *it);
extern void func_800B1B58(Point *, unsigned short);
extern void func_800B834C(void *center, s32 radius, unsigned short color), func_800B6728(Region *, Rect *);
extern void func_800B8138(Object *, Point *, unsigned char *, s32), func_800B17A4(void);
extern unsigned char func_800C57A0(void *rng);
static inline void position(Point *out, Point *point) { out->x = point->x; out->y = point->y; }
void func_800C2040(Object *arg)
{
    Rect rects[2];
    Point centers[2];
    Point low, high;
    Iterator iterator;
    Point point;
    unsigned char direction;
    s32 i;
    { s32 j; for (j = 1; j != -1; j--) {} }
    { s32 j; for (j = 1; j != -1; j--) {} }
    i = 0;
    centers[0].y = 0x18;
    centers[1].y = 0x34;
    centers[1].x = 0x1B;
    centers[0].x = 0x1B;
    for (;;) {
        s32 more = i < 2;
        s32 radius;
        unsigned short tile;
        Point *center;
        if (!more) break;
        center = &centers[i];
        radius = (unsigned char)func_800C5844(D_80147620, 5, 10);
        low.y = center->y - radius + 1;
        low.x = center->x - radius + 1;
        high.y = center->y + radius - 1;
        high.x = center->x + radius - 1;
        rects[i].low = low;
        rects[i].high = high;
        position(&point, &rects[i].low);
        iterator.first = point;
        iterator.current = iterator.first;
        position(&point, &rects[i].high);
        iterator.last = point;
        tile = (i & 15) | 0x1000;
        for (;;) {
            s32 more = iterator.current.x <= iterator.last.x;
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
