#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;


typedef struct Map800BAB28 {
    u8 pad_000[0x9BC];
    MapRect room_9BC;
} Map800BAB28;

extern s32 func_800A3050(MapRect *);
MapPos *func_800A33DC(MapPos *r, MapRect *q);

/* Explicit output in a0; returns out after selecting a room position or (0,0). */
MapPos *func_800BAB28(MapPos *out, Map800BAB28 *map) {
    MapPos result;
    MapRect *room = &map->room_9BC;

    if (func_800A3050(room) != 0) {
        result.x = 0;
        result.y = 0;
    } else {
        MapPos centre;

        func_800A33DC(&centre, room);
        result = centre;
    }
    out->x = result.x;
    out->y = result.y;
    return out;
}
