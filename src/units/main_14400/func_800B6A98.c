#include "common.h"

typedef unsigned char u8;

typedef struct Pos800B6A98 {
    s32 x;
    s32 y;
} Pos800B6A98;

/* Rectangle with per-side edge lengths at 0x10 (same Rect layout as func_800BCB18). */
typedef struct Rect800B6A98 {
    Pos800B6A98 tl;
    Pos800B6A98 br;
    u8 side_len_10[4];
} Rect800B6A98;

/* Edge iterator filled by func_800C25F4 (see func_800BCB18). */
typedef struct EdgeIter800B6A98 {
    u8 pad_00[0xC];
    s32 count;
    s32 index;
    u8 pad_14[4];
} EdgeIter800B6A98;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;


/* Inline edge-iterator constructor: clears the cursor and count, returns the iterator. */
static inline EdgeIter800B6A98 *edge_init(EdgeIter800B6A98 *e)
{
    e->index = 0;
    e->count = 0;
    return e;
}
void func_800C25F4(EdgeIter800B6A98 *it, Rect800B6A98 *rect, s32 side, s32 arg3);
Pos800B6A98 *func_800C2758(Pos800B6A98 *out, EdgeIter800B6A98 *it);
u32 func_800B1C6C(Pos800B6A98 *pos);
Pos800B6A98 *func_800A33DC(Pos800B6A98 *out, Rect800B6A98 *rect);

/* Pick the value-th flagged (0x800) cell along the room's edges, starting on the side that
 * contains the value; fall back to func_800A33DC when the edge runs out. */
Pos800B6A98 *func_800B6A98(Pos800B6A98 *out, Rect800B6A98 *rect, s32 value)
{
    EdgeIter800B6A98 edge;
    Pos800B6A98 pos;
    EdgeIter800B6A98 *it;
    s32 side;

    for (side = 0; side < 4; side++) {
        if (value < rect->side_len_10[side]) {
            break;
        }
        value -= rect->side_len_10[side];
    }
    if (side >= 4) {
        side = 0;
    }
    func_800C25F4(edge_init(&edge), rect, side, (D_80142F18.mode & 0xE0) == 0x40);
    it = &edge;
    for (;;) {
        /* ODD_C: block-scoped flags; it also keeps the exhaustion test at the loop top. */
        u32 flags;
        if (it->index >= it->count) break;
        func_800C2758(&pos, it);
        flags = func_800B1C6C(&pos);
        if ((flags & 0x800) && value-- <= 0) {
            out->x = pos.x;
            out->y = pos.y;
            goto done; /* found: skip the fallback */
        }
    }
    func_800A33DC(out, rect);
done:
    return out;
}
