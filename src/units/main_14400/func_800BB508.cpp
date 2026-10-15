#include "common.h"
#include "map_line_iterator.h"
#include "map_edge_iterator.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern "C" {
s32 func_800A251C(MapPos *a, MapPos *b);
void func_800B1B58(MapPos *pos, u16 flags);
void func_800B1BE0(MapPos *pos, s32 flags);
}
static inline s32 line_active(MapLineIterator *line) { return line->index < line->length; }
static inline MapLineIterator *start_line(MapLineIterator *line, MapPos *origin, s32 direction, s32 length) { u8 byte = direction; func_800C25D0(line, origin, &byte, length); return line; }
/* Callers supply the generator receiver; this operation only needs the points. */
extern "C" void func_800BB508(void *self, MapPos *origin, MapPos *corner1, MapPos *corner2) {
    if (func_800A251C(origin, corner1)) {
        MapLineIterator iterator;
        MapPos vertical;
        s32 a = origin->y, b = corner2->y;
        s32 direction = (b < a) << 2;
        s32 count;
        MapLineIterator *line;
        if (a <= b) count = b - a + 1;
        else count = a - b + 1;
        line = start_line(&iterator, origin, direction, count);
        while (line_active(line)) {
            func_800C2758(&vertical, line);
            func_800B1B58(&vertical, 0x4000);
            func_800B1BE0(&vertical, 0x200);
        }
    } else if (func_800A251C(origin, corner2)) {
        MapLineIterator iterator;
        MapPos horizontal;
        s32 a = origin->x, b = corner1->x;
        s32 direction = a <= b ? 6 : 2;
        s32 count;
        MapLineIterator *line;
        if (a <= b) count = b - a + 1;
        else count = a - b + 1;
        line = start_line(&iterator, origin, direction, count);
        while (line_active(line)) {
            func_800C2758(&horizontal, line);
            func_800B1B58(&horizontal, 0x4000);
            func_800B1BE0(&horizontal, 0x200);
        }
    } else {
        MapEdgeIterator edge;
        MapPos point;
        func_800A3670(&edge, origin, corner1, corner2);
        while (func_800A375C(&edge)) {
            func_800A3834(&point, &edge);
            func_800B1B58(&point, 0x4000);
            func_800B1BE0(&point, 0x200);
        }
    }
}
