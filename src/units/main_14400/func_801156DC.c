#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;

extern Pos func_800B5A18(Pos *pos, Dir dir, s32 count, u16 mask);
extern void func_800A2758(Pos *p, Dir d);

/* Trace from position along dir for range cells, then step once more; result to out. */
void *func_801156DC(Pos *out, void *owner /* unused supplied receiver */, Pos *position, Dir *dir, s32 range) {
    Pos result;
    Pos start;
    Dir heading;

    start.x = position->x;
    start.y = position->y;
    heading = *dir;
    result = func_800B5A18(&start, heading, range, 0xC000);
    func_800A2758(&result, *dir);
    out->x = result.x;
    out->y = result.y;
    return out;
}
