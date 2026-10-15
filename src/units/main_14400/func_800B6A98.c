#include "common.h"
#include "map_line_iterator.h"

typedef unsigned char u8;

/* Complete 20-byte room record: geometry followed by four per-side counts. */
typedef struct Rect800B6A98 {
    MapRect geometry;
    u8 side_len_10[4];
} Rect800B6A98;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;


/* Inline edge-iterator constructor: clears the cursor and length, returns the iterator. */
static inline MapLineIterator *edge_init(MapLineIterator *e)
{
    e->index = 0;
    e->length = 0;
    return e;
}
u32 func_800B1C6C(MapPos *pos);
MapPos *func_800A33DC(MapPos *out, MapRect *rect);

/* Pick the value-th flagged (0x800) cell along the room's edges, starting on the side that
 * contains the value; fall back to func_800A33DC when the edge runs out. */
MapPos *func_800B6A98(MapPos *out, Rect800B6A98 *rect, s32 value)
{
    MapLineIterator edge;
    MapPos pos;
    MapLineIterator *it;
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
    func_800C25F4(edge_init(&edge), &rect->geometry, side, (D_80142F18.mode & 0xE0) == 0x40);
    it = &edge;
    for (;;) {
        /* ODD_C: block-scoped flags; it also keeps the exhaustion test at the loop top. */
        u32 flags;
        if (it->index >= it->length) break;
        func_800C2758(&pos, it);
        flags = func_800B1C6C(&pos);
        if ((flags & 0x800) && value-- <= 0) {
            out->x = pos.x;
            out->y = pos.y;
            goto done; /* found: skip the fallback */
        }
    }
    func_800A33DC(out, &rect->geometry);
done:
    return out;
}
