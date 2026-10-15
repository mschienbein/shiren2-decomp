#include "map_rect.h"

typedef unsigned char u8;

/* The object is used through casts rather than a typed local copy: a local
 * `u8 *self = object;` moves rect into its saved register before object
 * (0x800C00BC/0x800C00C4 save a0 first). */
s32 func_800C00B4(void *object, const MapRect *rect) {
    s32 i = 0;
    s32 offset = 0;

    while (1) {
        if (i >= *(s32 *)((u8 *)object + 0x3DC)) {
            break;
        }
        /* The computed address denotes the bounds of one 0x14-byte room. */
        if (func_800A32D8(rect, (const MapRect *)((u8 *)(offset + (s32)object) + 0x404))) {
            return 1;
        }
        offset += 0x14;
        i++;
    }
    return 0;
}
