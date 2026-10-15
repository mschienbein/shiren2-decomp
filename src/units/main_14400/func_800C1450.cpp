#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;

/* Borrowed floor-generator prefix; no Floor object is constructed here. */
struct Floor_800C1450 {
    char pad[0x3DC];
    s32 room_count;
};

extern "C" {
extern u8 D_80143392;
extern u8 D_8014344C;
extern u8 D_80147620[];
/* Per-room bounds parameters have four-byte rows; rows 0..7 are used. */
extern const u8 D_80153DD8[][4];
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B17A4(void);
}

/* Keep the original reference-based assignment and its two native corner copies. */
static inline MapRect &assign_room(MapRect &destination, const MapRect &source)
{
    destination.min = source.min;
    destination.max = source.max;
    return destination;
}

/* The painter receives the address of a distinct copy-constructed value. */
static inline void paint_room(Floor_800C1450 *floor, s32 index, MapRect bounds)
{
    func_800B7948(floor, index, &bounds);
}

extern "C" void func_800C1450(Floor_800C1450 *floor)
{
    MapRect room;
    MapRect rooms[4];
    for (s32 i = 0; i < 4; i++) {
        room.min.y = (u8)func_800C5844(D_80147620, D_80153DD8[0][i], D_80153DD8[1][i]) + 10;
        room.min.x = (u8)func_800C5844(D_80147620, D_80153DD8[2][i], D_80153DD8[3][i]) + 10;
        room.max.y = (u8)func_800C5844(D_80147620, D_80153DD8[4][i], D_80153DD8[5][i]) + 10;
        room.max.x = (u8)func_800C5844(D_80147620, D_80153DD8[6][i], D_80153DD8[7][i]) + 10;
        assign_room(rooms[i], room);
        paint_room(floor, i, rooms[i]);
    }
    func_800B7E9C(floor, &rooms[0], &rooms[1]);
    func_800B7E9C(floor, &rooms[2], &rooms[3]);
    func_800B7E9C(floor, &rooms[0], &rooms[2]);
    func_800B7E9C(floor, &rooms[1], &rooms[3]);
    floor->room_count = 4;
    D_8014344C = 4;
    func_800B17A4();
    D_80143392 = 0;
}
