#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x, y; } Position;
typedef struct {
    Position origin;
    u8 direction;
    u8 limit;
    u8 index;
    u8 mode;
    s16 offset_y;
    s16 offset_x;
    s32 held;
    s16 ring;
} Iterator;

extern Position *func_800C282C(Position *result, Iterator *it);
extern u32 func_800B1C6C(Position *position);

static inline Position *copy(Position *dest, const Position *source)
{
    dest->x = source->x;
    dest->y = source->y;
    return dest;
}

Position *func_800C28FC(Position *result, Iterator *it)
{
    Position p;
    u32 prev;

    func_800C282C(&p, it);
    prev = it->index;
    if (prev < it->limit) {
        if ((it->direction ^ 1) & 1) {
            s32 v = it->offset_y;
            if (v == 0) {
                it->offset_y++;
            } else if (v > 0) {
                it->offset_y = -v;
            } else {
                it->offset_y = -v + 1;
            }
            if (it->offset_y > it->ring) {
                it->offset_y = 0;
                it->offset_x++;
                it->ring++;
                it->index++;
            }
        } else if (it->mode == 0) {
            if (it->offset_y <= it->offset_x) {
                it->ring++;
                it->offset_y = it->index;
                it->offset_x = it->index - it->ring;
            } else {
                it->offset_y = it->index - it->ring;
                it->offset_x = it->index;
            }
            if (it->ring > it->index) {
                it->index++;
                it->ring = 0;
                it->offset_y = it->index;
                it->offset_x = it->index;
            }
        } else {
            s32 e = it->offset_x;
            if (e != 0) {
                it->offset_x = it->offset_y;
                it->offset_y = e;
                if (it->offset_x >= e) {
                    it->offset_y = e - 1;
                    it->offset_x = it->ring - it->offset_y;
                }
            } else {
                s32 n;
                it->ring++;
                n = (u16)it->ring;
                it->index = prev + 1;
                if (!(n & 1)) {
                    s16 half = (s16)n / 2;
                    it->offset_x = half;
                    it->offset_y = half;
                } else {
                    it->offset_y = (s16)n / 2;
                    it->offset_x = it->ring - it->offset_y;
                }
            }
        }
    }
    if (!(func_800B1C6C(&p) & 0x8000)) {
        it->held = 0;
    }
    if (prev != it->index) {
        if (it->held != 0) {
            it->index = it->limit;
        } else {
            it->held = 1;
        }
    }
    return copy(result, &p);
}
