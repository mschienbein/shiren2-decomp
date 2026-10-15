#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x3DC]; s32 field_3DC; } Obj800C0B40;
extern MapRect D_801429C0;
extern u8 D_80147620[];
extern u8 D_80156AC1;
extern u8 D_80156AC3;
extern u16 D_80156AC4;
extern u16 D_80156AC6;
extern u8 D_8014344C;
extern u8 D_80143392;
s32 func_800C5844(void *rng, u8 lo, u8 hi);
u32 func_800B1C6C(MapPos *pos);
void func_800B1B58(MapPos *pos, u16 flags);
void func_800B1BE0(MapPos *pos, s32 flags);
void func_800B17A4(void);

static inline s32 u8_less(u8 a, u8 b) {
    return a < b;
}

/* Whole-object accessors for D_801429C0 (wave-4 rule 4): direct member reads
   let CSE share one base register, the accessors keep one %hi/%lo per field. */
static inline s32 rect_x0(MapRect *r) { return r->min.x; }
static inline s32 rect_y0(MapRect *r) { return r->min.y; }
static inline s32 rect_x1(MapRect *r) { return r->max.x; }
static inline s32 rect_y1(MapRect *r) { return r->max.y; }

static inline s32 iter_valid(MapPositionIterator *it) {
    return it->cur.x <= it->end.x;
}

void func_800C0B40(Obj800C0B40 *obj) {
    MapRect rect;
    MapRect *r;
    MapPositionIterator it;
    MapPos pos;
    MapPositionIterator fill;
    u8 count;
    u8 room;
    u8 attempt;
    s32 width;
    s32 height;
    s32 max_w;
    s32 max_h;
    s32 blocked;

    r = &rect;
    r->min.x = rect_x0(&D_801429C0);
    r->min.y = rect_y0(&D_801429C0);
    rect.max.x = rect_x1(&D_801429C0);
    rect.max.y = rect_y1(&D_801429C0);
    func_800B7948(obj, 0, r);
    max_w = D_80156AC4 + 2;
    max_h = D_80156AC6 + 2;
    count = (u8)func_800C5844(D_80147620, D_80156AC1, D_80156AC3);
    for (room = 0; u8_less(room, count); room++) {
        for (attempt = 0; u8_less(attempt, 10); attempt++) {
            width = (u8)func_800C5844(D_80147620, max_w, max_h);
            height = (u8)func_800C5844(D_80147620, max_w, max_h);
            rect.min.y = (u8)func_800C5844(D_80147620, 10, 0x41);
            rect.min.x = (u8)func_800C5844(D_80147620, 10, 0x2B);
            rect.max.y = rect.min.y + width - 1;
            if (rect.max.y >= 0x42) {
                continue;
            }
            rect.max.x = rect.min.x + height - 1;
            if (rect.max.x >= 0x2C) {
                continue;
            }
            blocked = 0;
            it.start = rect.min;
            it.cur = it.start;
            it.end = rect.max;
            while (iter_valid(&it)) {
                func_800A3610(&pos, &it);
                if (func_800B1C6C(&pos) & 0x4000) {
                    blocked = 1;
                    break;
                }
            }
            if (blocked) {
                continue;
            }
            rect.min.y++;
            rect.min.x++;
            rect.max.y--;
            rect.max.x--;
            fill.start = rect.min;
            fill.cur = fill.start;
            fill.end = rect.max;
            while (iter_valid(&fill)) {
                func_800A3610(&pos, &fill);
                func_800B1B58(&pos, 0x4000);
                func_800B1BE0(&pos, 0x200);
            }
            break;
        }
    }
    obj->field_3DC = 1;
    D_8014344C = 1;
    func_800B17A4();
    D_80143392 = 5;
}
