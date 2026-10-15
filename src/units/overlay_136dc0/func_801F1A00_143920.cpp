extern "C" {
#include "func_801F1A00_143920/types.h"
}

/* ODD_C: complement the callee's proven s32 0/1 result inside the predicate.
 * The direct branch expression in the first repaired receipt was folded to
 * comparisons with 1, changed saved-register lifetimes, and differed by 57 words.
 * This is a real logical operation; the external return type remains s32. */
static inline s32 requirement_fails(u8 id, u8 low, u8 high) {
    return func_801F1974_143894(id, low, high) ^ 1;
}

extern "C" void func_801F1A00_143920(s32 announce, s32 first, s32 last) {
    Event801F1A00 event;
    u8 placed[0xD7];
    s32 i;
    Spawn801F1A00 *spawn;

    func_800265E0(placed, sizeof(placed));
    for (i = first; i < last; i++) {
        func_801E5BE4_137B04((u32)i, 0x16, (u8 *)&event);
        if (event.floor != -1 && event.floor != D_80142F18.coordinates[0]) {
            continue;
        }
        if (requirement_fails(event.req0_id, event.req0_low, event.req0_high)) {
            continue;
        }
        if (requirement_fails(event.req1_id, event.req1_low, event.req1_high)) {
            continue;
        }
        if (event.coordinates.packed == 0) {
            placed[event.id] = 1;
            continue;
        }
        if (func_801F2E30_144D50(event.id)) {
            continue;
        }
        if (placed[event.id] == 1) {
            continue;
        }
        spawn = func_801F1510_143430(&event, 1, 0);
        if (spawn != 0) {
            placed[event.id] = 1;
            func_801F1B58_143A78(spawn, &event);
            if (announce) {
                spawn->field_52 = 1;
            }
        }
    }
}
