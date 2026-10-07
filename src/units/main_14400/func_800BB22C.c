#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 x; s32 y; } Point800BB22C;
typedef struct { Point800BB22C a; Point800BB22C b; } Rect800BB22C;
typedef struct { Point800BB22C cur; Point800BB22C start; Point800BB22C end; } RectIter800BB22C;
extern u8 D_80147620[];
void *func_800A3610(void *out, void *it);
u32 func_800B1C6C(void *pos);
u8 func_800C57A0(void *rng);
void func_800B1AE0(Point800BB22C *pos, u16 flags);
void func_800B17A4(void);

static inline void initIter800BB22C(RectIter800BB22C *it, Rect800BB22C *rect) {
    it->start = rect->a;
    it->cur = it->start;
    it->end = rect->b;
}

static inline s32 iterValid800BB22C(RectIter800BB22C *it) {
    return it->cur.x <= it->end.x;
}

s32 func_800BB22C(void *self, Rect800BB22C *area) {
    Point800BB22C pos;
    Rect800BB22C rect;
    RectIter800BB22C it1;
    Rect800BB22C line;
    RectIter800BB22C it2;
    s32 canUp;
    s32 canDown;
    u32 flags;

    rect.a.x = area->a.x;
    rect.a.y = area->a.y;
    rect.b.x = area->b.x;
    rect.b.y = area->b.y;
    line.a.x = rect.a.x;
    line.a.y = rect.a.y - 2;
    line.b.x = rect.b.x;
    line.b.y = rect.a.y - 1;
    canUp = 0;
    if (rect.a.y - 1 >= 10) {
        canUp = 1;
    }
    canDown = 0;
    if (rect.b.y + 1 < 0x42) {
        canDown = 1;
    }

    initIter800BB22C(&it1, &line);
    while (iterValid800BB22C(&it1)) {
        func_800A3610(&line.a, &it1);
        if (!(func_800B1C6C(&line.a) & 0xE100)) {
            canUp = 0;
            break;
        }
    }

    line.a.x = rect.a.x;
    line.a.y = rect.b.y + 1;
    line.b.x = rect.b.x;
    line.b.y = rect.b.y + 2;
    initIter800BB22C(&it2, &line);
    while (iterValid800BB22C(&it2)) {
        func_800A3610(&line.a, &it2);
        if (!(func_800B1C6C(&line.a) & 0xE100)) {
            canDown = 0;
            break;
        }
    }

    if (!canUp && !canDown) {
        return 0;
    }
    flags = func_800B1C6C(&rect.a);
    if (canUp && (!canDown || (func_800C57A0(D_80147620) & 1))) {
        s32 y = rect.a.y - 1;

        pos.y = y;
        rect.a.y = y;
    } else {
        s32 y = rect.b.y + 1;

        pos.y = y;
        rect.b.y = y;
    }
    for (pos.x = rect.a.x; pos.x <= rect.b.x; pos.x++) {
        func_800B1AE0(&pos, flags);
    }
    area->a = rect.a;
    area->b = rect.b;
    func_800B17A4();
    return 1;
}
