#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern u8 D_80147620[];
u32 func_800B1C6C(MapPos *pos);
u8 func_800C57A0(void *rng);
void func_800B1AE0(MapPos *pos, u16 flags);
void func_800B17A4(void);

static inline void initIter800BB22C(MapPositionIterator *it, MapRect *rect) {
    it->start = rect->min;
    it->cur = it->start;
    it->end = rect->max;
}

static inline s32 iterValid800BB22C(MapPositionIterator *it) {
    return it->cur.x <= it->end.x;
}

s32 func_800BB22C(void *self, MapRect *area) {
    MapPos pos;
    MapRect rect;
    MapPositionIterator it1;
    MapRect line;
    MapPositionIterator it2;
    s32 canUp;
    s32 canDown;
    u32 flags;

    rect.min.x = area->min.x;
    rect.min.y = area->min.y;
    rect.max.x = area->max.x;
    rect.max.y = area->max.y;
    line.min.x = rect.min.x;
    line.min.y = rect.min.y - 2;
    line.max.x = rect.max.x;
    line.max.y = rect.min.y - 1;
    canUp = 0;
    if (rect.min.y - 1 >= 10) {
        canUp = 1;
    }
    canDown = 0;
    if (rect.max.y + 1 < 0x42) {
        canDown = 1;
    }

    initIter800BB22C(&it1, &line);
    while (iterValid800BB22C(&it1)) {
        func_800A3610(&line.min, &it1);
        if (!(func_800B1C6C(&line.min) & 0xE100)) {
            canUp = 0;
            break;
        }
    }

    line.min.x = rect.min.x;
    line.min.y = rect.max.y + 1;
    line.max.x = rect.max.x;
    line.max.y = rect.max.y + 2;
    initIter800BB22C(&it2, &line);
    while (iterValid800BB22C(&it2)) {
        func_800A3610(&line.min, &it2);
        if (!(func_800B1C6C(&line.min) & 0xE100)) {
            canDown = 0;
            break;
        }
    }

    if (!canUp && !canDown) {
        return 0;
    }
    flags = func_800B1C6C(&rect.min);
    if (canUp && (!canDown || (func_800C57A0(D_80147620) & 1))) {
        s32 y = rect.min.y - 1;

        pos.y = y;
        rect.min.y = y;
    } else {
        s32 y = rect.max.y + 1;

        pos.y = y;
        rect.max.y = y;
    }
    for (pos.x = rect.min.x; pos.x <= rect.max.x; pos.x++) {
        func_800B1AE0(&pos, flags);
    }
    area->min = rect.min;
    area->max = rect.max;
    func_800B17A4();
    return 1;
}
