#include "func_801F1F64_143E84/types.h"

static inline s32 part_ready(Part801F1F64 *part) {
    return part->state == 1;
}

void func_801F1F64_143E84(Actor801F1F64 *actor) {
    if ((actor->flags_1E >> 6) & 1) {
        if (!part_ready(actor ? &actor->parts.part_78 : 0)) {
            func_80049CB4(0x91, actor);
        }
    }
    if ((actor->flags_1E >> 3) & 1) {
        if (!part_ready(actor ? &actor->parts.inner.part_84 : 0)) {
            func_80049CB4(0x91, actor);
        }
    }
}
