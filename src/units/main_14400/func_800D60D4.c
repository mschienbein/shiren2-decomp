#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 value;
} Dir;

s32 func_800A23E8(Pos *origin, Pos *vec);
void *func_800A27A4(void *outDirection, void *from, void *to);

static inline void copy_pos(Pos *out, Pos *in)
{
    out->x = in->x;
    out->y = in->y;
}

static inline s32 is_cardinal(Dir *dir)
{
    return (dir->value & 1) == 0;
}

/* Fill `out` with candidate step directions from `from` toward `to` while facing `dir`,
   best first; returns how many were written. */
u8 func_800D60D4(Pos *from, Pos *to, Dir dir, u8 *out)
{
    Pos target;
    Dir step;
    s32 turn = -1;
    s32 n = 0;
    s32 count = 3;
    s32 dy = to->y - from->y;
    s32 dx = to->x - from->x;
    u8 value;
    s32 i;

    if (is_cardinal(&dir)) {
        count = 5;
    }
    copy_pos(&target, to);
    if (func_800A23E8(from, &target) == 1) {
        copy_pos(&target, to);
        func_800A27A4(&step, from, &target);
        out[0] = step.value;
        n = 1;
        count++;
    }
    value = dir.value;
    switch (value) {
    case 2:
        if (dy > 0) {
            value = 1;
            turn = 1;
        } else if (dy < 0) {
            value = 3;
        }
        break;
    case 6:
        if (dy > 0) {
            value = 7;
        } else if (dy < 0) {
            value = 5;
            turn = 1;
        }
        break;
    case 4:
        if (dx > 0) {
            value = 5;
        } else if (dx < 0) {
            value = 3;
            turn = 1;
        }
        break;
    case 0:
        if (dx > 0) {
            value = 7;
            turn = 1;
        } else if (dx < 0) {
            value = 1;
        }
        break;
    }
    out[n++] = value;
    out[n++] = value + turn;
    out[n++] = value - turn;
    out[n++] = value + turn * 2;
    out[n] = value - turn * 2;
    for (i = (u8)count; --i != -1;) {
        out[i] &= 7;
    }
    return count;
}
