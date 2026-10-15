#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef signed char s8;

extern "C" {
extern MapPos D_80148000;
s32 func_800A99D0(void);
s32 func_800D6784(MapPos *best, u8 *flag);
s32 func_800D6430(MapPos *best, u8 *flag);
s32 func_800D68EC(MapPos *out, s8 *flag);
s32 func_800D6D04(void *map, MapPos *pos, MapPos *best);
}

static inline s8 choose_point(s32 a, s32 b, MapPos *second, MapPos *first) {
    if (a) {
        if (b) {
            return func_800D6D04(&D_80148000, second, first) != 0;
        } else {
            return 0;
        }
    } else {
        s32 result = -1;
        if (b) {
            result = 1;
        }
        return result;
    }
}

extern "C" s32 func_800D6318(MapPos *out, u8 *tag) {
    s32 status[2];
    u8 tags[2];
    /* Native construction of the two candidates; original countdown at
     * VRAM 0x800D632C..0x800D6348 precedes the first producer call. */
    MapPos points[2];
    MapPos *second, *first;
    s8 choice;

    if (func_800A99D0()) {
        status[0] = func_800D6784(&points[0], &tags[0]);
    } else {
        status[0] = func_800D6430(&points[0], &tags[0]);
    }
    second = &points[1];
    first = &points[0];
    status[1] = func_800D68EC(second, (s8 *)&tags[1]);
    choice = choose_point(status[0], status[1], second, first);
    if (choice != -1) {
        *out = points[choice];
        *tag = tags[choice];
        return 1;
    }
    return 0;
}
