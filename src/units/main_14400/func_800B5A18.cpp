#include "common.h"
#include "map_types.h"
typedef unsigned short u16;

/* MapPos (map_types.h) has a user-declared member-wise copy constructor, so the result is
   returned by value through the hidden result pointer; the MapDir byte is passed by value. */

extern "C" {
void func_800A2758(MapPos *pos, MapDir dir);
u32 func_800B1C6C(MapPos *pos);
}

/* Step pos along dir up to count cells; on hitting a cell with a mask flag, step back
   once and stop. Returns the final position. */
extern "C" MapPos func_800B5A18(MapPos *pos, MapDir dir, s32 count, u16 mask) {
    s32 i = 0;
    for (;;) {
        u16 hit;
        if (i >= count) break;
        func_800A2758(pos, dir);
        hit = mask & func_800B1C6C(pos);
        if (hit) {
            MapDir *dp = &dir;
            MapDir back;
            back.value = (dp->value + 4) & 7;
            func_800A2758(pos, back);
            break;
        }
        i++;
    }
    return *pos;
}
