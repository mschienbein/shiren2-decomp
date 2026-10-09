#include "common.h"

typedef unsigned char u8;

typedef struct Vec2 {
    s32 x;
    s32 y;
} Vec2;

typedef struct Rect800BAB28 {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} Rect800BAB28;

typedef struct Map800BAB28 {
    u8 pad_000[0x9BC];
    Rect800BAB28 room_9BC;
} Map800BAB28;

extern s32 func_800A3050(void *);
Vec2 *func_800A33DC(Vec2 *r, Rect800BAB28 *q);

/* Struct-return helper (hidden result pointer in a0): centre of the map's room, or (0,0). */
Vec2 *func_800BAB28(Vec2 *out, Map800BAB28 *map) {
    Vec2 result;
    Rect800BAB28 *room = &map->room_9BC;

    if (func_800A3050(room) != 0) {
        result.x = 0;
        result.y = 0;
    } else {
        Vec2 centre;

        func_800A33DC(&centre, room);
        result = centre;
    }
    out->x = result.x;
    out->y = result.y;
    return out;
}
