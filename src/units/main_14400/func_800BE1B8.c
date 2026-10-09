#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct Owner { u8 pad_00[0x402]; u16 flags_402; } Owner;
extern u8 D_80147620[];
extern void func_800B1BE0(Pos *pos, s32 mask);
extern void func_800B1B58(Pos *pos, u16 flags);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800A2758(Pos *pos, Dir direction);
extern s32 func_801368B4(Pos *pos, s32 mask);
static __inline__ Dir *direction(Dir *result, s32 value) {
    result->value = value & 7;
    return result;
}
static __inline__ Pos *copy_position(Pos *out, Pos *source) {
    out->x = source->x;
    out->y = source->y;
    return out;
}
static __inline__ s32 paintable(Pos *pos) {
    s32 result = 0;
    if (func_801368B4(pos, 0x4000) && !func_801368B4(pos, 0x8020)) result = 1;
    return result;
}
void func_800BE1B8(Owner *owner, Pos *start, Pos *end, s32 begin_y, s32 force) {
    Pos pos;
    Dir vertical, horizontal;
    s32 positive_y = start->y < end->y;
    s32 positive_x = start->x < end->x;
    u8 direction_y = positive_y ? 0 : 4;
    u8 direction_x = positive_x ? 6 : 2;
    u8 limit_y, limit_x;
    u8 count;
    s32 done_y, done_x;
    func_800B1BE0(copy_position(&pos, start), 0x4000);
    func_800B1B58(&pos, owner->flags_402 | 0x200);
    limit_y = (positive_y ? end->y - start->y : start->y - end->y) / 4;
    if (limit_y < 2) limit_y = 2;
    limit_x = (positive_x ? end->x - start->x : start->x - end->x) / 4;
    if (limit_x < 2) limit_x = 2;
    done_y = 0;
    do {
        if (!begin_y) {
            begin_y = 1;
        } else {
            u8 distance = positive_y ? (u8)end->y - (u8)pos.y : (u8)pos.y - (u8)end->y;
            done_y = distance == 0;
            if (!done_y) {
                u8 base = 2, top;
                if (distance < 2) base = 1;
                top = limit_y;
                if (distance < top) top = distance;
                count = func_800C5844(D_80147620, base, top);
                for (;;) {
                    /* ODD_C: block-scoped step value keeps the count test at the loop top. */
                    u8 left;
                    if ((left = --count) == 255) break;
                    func_800A2758(&pos, *direction(&vertical, direction_y));
                    if (force || paintable(&pos)) {
                        func_800B1BE0(&pos, 0x4000);
                        func_800B1B58(&pos, owner->flags_402 | 0x200);
                    }
                }
            }
        }
        {
            u8 distance = positive_x ? (u8)end->x - (u8)pos.x : (u8)pos.x - (u8)end->x;
            done_x = distance == 0;
            if (!done_x) {
                u8 base = 2, top;
                if (distance < 2) base = 1;
                top = limit_x;
                if (distance < top) top = distance;
                count = func_800C5844(D_80147620, base, top);
                for (;;) {
                    /* ODD_C: block-scoped step value keeps the count test at the loop top. */
                    u8 left;
                    if ((left = --count) == 255) break;
                    func_800A2758(&pos, *direction(&horizontal, direction_x));
                    if (force || paintable(&pos)) {
                        func_800B1BE0(&pos, 0x4000);
                        func_800B1B58(&pos, owner->flags_402 | 0x200);
                    }
                }
            }
        }
    } while (!done_y || !done_x);
}
