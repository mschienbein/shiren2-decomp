#include "common.h"
#include "map_rect.h"
typedef unsigned char u8;


struct Room_800BF5C4 {
    MapRect bounds;
    u8 sideCounts[4];
};

struct Obj_800BF5C4 {
    u8 pad[0x3DC];
    s32 count;
};

extern "C" {
extern Room_800BF5C4 D_801431F0[];
extern u8 D_80147620[];
s32 func_800C587C(void *rng, u8 chance);
u32 func_800B1C6C(MapPos *pos);
void func_800BC69C(Obj_800BF5C4 *obj, MapRect *room, s32 mode);
}

static inline int bits_clear(u32 value, u32 mask)
{
    if (value & mask) {
        return 0;
    }
    return 1;
}

extern "C" void func_800BF5C4(Obj_800BF5C4 *obj)
{
    s32 i = 0;
    while (i < obj->count) {
        int ok = 0;
        Room_800BF5C4 *room = &D_801431F0[i];
        s32 y0 = room->bounds.min.y;
        s32 x0 = room->bounds.min.x;
        s32 y1 = room->bounds.max.y;
        s32 x1 = room->bounds.max.x;
        if (func_800C587C(D_80147620, 10)) {
            MapPos p1(x0, y0);
            if (bits_clear(func_800B1C6C(&p1), 0x4000)) {
                MapPos p2(x0, y1);
                if (bits_clear(func_800B1C6C(&p2), 0x4000)) {
                    MapPos p3(x1, y0);
                    if (bits_clear(func_800B1C6C(&p3), 0x4000)) {
                        MapPos p4(x1, y1);
                        ok = bits_clear(func_800B1C6C(&p4), 0x4000);
                    }
                }
            }
        }
        if (ok) {
            func_800BC69C(obj, &room->bounds, 1);
        }
        i++;
    }
}
