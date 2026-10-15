#include "map_rect.h"

typedef struct { char pad[0x2DC]; MapRect entries[16]; s32 count3DC; } Obj;
/* The object is used through casts rather than a typed local copy: a local
 * `Obj *obj = object;` moves rect into its saved register before object
 * (0x800B9FE8/0x800B9FF0 save a0 first). */
s32 func_800B9FE0(void *object, const MapRect *rect) {
    s32 i = 0;
next:
    if (i < ((Obj *)object)->count3DC) {
        /* local-arithmetic-qualification: entry i has stride 16 at object+0x2DC;
         * pointer arithmetic reverses the original addu a1,a1,s1 operands.
         * The computed address must designate an existing entries[i]. */
        if (func_800A32D8(rect, (const MapRect *)(i * 16 + (u32)object + 0x2DC))) {
            return 1;
        }
        i++;
        goto next;
    }
    return 0;
}
