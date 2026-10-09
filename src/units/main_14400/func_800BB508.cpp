#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair current; u8 direction; s32 length, index; } Line;
typedef struct { Pair current; char pad8[8]; Pair min, max; double slope, intercept; s32 positive, negative; } Edge;
extern "C" {
s32 func_800A251C(Pair *a, Pair *b);
void func_800C25D0(Line *obj, Pair *pair, u8 *byte, s32 value);
void *func_800C2758(void *out, void *iterator);
void func_800B1B58(Pair *pos, u16 flags);
void func_800B1BE0(Pair *pos, s32 flags);
Edge *func_800A3670(Edge *e, Pair *a, Pair *b, Pair *c);
s32 func_800A375C(Edge *e);
Pair *func_800A3834(Pair *out, Edge *e);
}
static inline s32 line_active(Line *line) { return line->index < line->length; }
static inline Line *start_line(Line *line, Pair *origin, s32 direction, s32 length) { u8 byte = direction; func_800C25D0(line, origin, &byte, length); return line; }
/* Callers supply the generator receiver; this operation only needs the points. */
extern "C" void func_800BB508(void *self, Pair *origin, Pair *corner1, Pair *corner2) {
    if (func_800A251C(origin, corner1)) {
        Line iterator;
        Pair vertical;
        s32 a = origin->y, b = corner2->y;
        s32 direction = (b < a) << 2;
        s32 count;
        Line *line;
        if (a <= b) count = b - a + 1;
        else count = a - b + 1;
        line = start_line(&iterator, origin, direction, count);
        while (line_active(line)) {
            func_800C2758(&vertical, line);
            func_800B1B58(&vertical, 0x4000);
            func_800B1BE0(&vertical, 0x200);
        }
    } else if (func_800A251C(origin, corner2)) {
        Line iterator;
        Pair horizontal;
        s32 a = origin->x, b = corner1->x;
        s32 direction = a <= b ? 6 : 2;
        s32 count;
        Line *line;
        if (a <= b) count = b - a + 1;
        else count = a - b + 1;
        line = start_line(&iterator, origin, direction, count);
        while (line_active(line)) {
            func_800C2758(&horizontal, line);
            func_800B1B58(&horizontal, 0x4000);
            func_800B1BE0(&horizontal, 0x200);
        }
    } else {
        Edge edge;
        Pair point;
        func_800A3670(&edge, origin, corner1, corner2);
        while (func_800A375C(&edge)) {
            func_800A3834(&point, &edge);
            func_800B1B58(&point, 0x4000);
            func_800B1BE0(&point, 0x200);
        }
    }
}
