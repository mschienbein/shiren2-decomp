#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;

/* Borrowed floor-generator view; only room_count is accessed in this TU. */
struct Floor_800C1290 {
    char pad[0x3DC];
    s32 room_count;
};

extern "C" {
extern u8 D_80143392;
extern u8 D_8014344C;
extern u8 D_80147620[];
/* Per-room bounds parameters have four-byte rows; rows 0..7 are used. */
extern const u8 D_80153DB4[][4];
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800B17A4(void);
}

/* Rectangle assignment corner by corner. The original copies rooms[i] = room as two
   8-byte MapPos copies (0x800C13A0..0x800C13BC, a3/t0 twice); the implicit MapRect
   assignment is one 16-byte block copy through four registers. */
static inline void assign(MapRect &dst, const MapRect &src) { dst.min = src.min; dst.max = src.max; }

/* Keep the by-value bounds copy alive while B7948 consumes its address. */
static inline void paint_room(Floor_800C1290 *floor, s32 index, MapRect bounds)
{
    func_800B7948(floor, index, &bounds);
}

extern "C" void func_800C1290(Floor_800C1290 *floor)
{
    MapRect room;
    MapRect rooms[2];
    for (s32 i = 0; i < 2; i++) {
        room.min.y = (u8)func_800C5844(D_80147620, D_80153DB4[0][i], D_80153DB4[1][i]) + 10;
        room.min.x = (u8)func_800C5844(D_80147620, D_80153DB4[2][i], D_80153DB4[3][i]) + 10;
        room.max.y = (u8)func_800C5844(D_80147620, D_80153DB4[4][i], D_80153DB4[5][i]) + 10;
        room.max.x = (u8)func_800C5844(D_80147620, D_80153DB4[6][i], D_80153DB4[7][i]) + 10;
        assign(rooms[i], room);
        paint_room(floor, i, rooms[i]);
    }
    func_800B7E9C(floor, &rooms[0], &rooms[1]);
    floor->room_count = 2;
    D_8014344C = 2;
    func_800B17A4();
    D_80143392 = 0;
}
