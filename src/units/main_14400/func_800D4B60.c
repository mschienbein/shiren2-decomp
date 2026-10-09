#include "common.h"

typedef signed char s8;
typedef struct { s32 x, y; } Buf;
typedef struct { unsigned char pad00[8]; s8 index08; } S;
typedef struct { s32 x, y, width, height; } Range;
extern Range D_80147F80[];
extern s32 func_800A315C(Range *range);

Buf *func_800D4B60(Buf *out, S *self, s32 index) {
    Buf point;
    Range *ranges = D_80147F80;
    s32 width = func_800A315C(&ranges[self->index08]);
    s32 row = index / width;
    Buf *pos = &point;
    Range *range;
    pos->x = index - row * width;
    pos->y = row;
    range = &ranges[self->index08];
    pos->x += range->x;
    pos->y += range->y;
    out->x = pos->x;
    out->y = pos->y;
    return out;
}
