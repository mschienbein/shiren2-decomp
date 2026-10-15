#include "func_801EDEF8_13FE18/types.h"

extern Rect D_801429C0;
s32 func_800AE2A4(void *obj, s32 allowItem, s32 allowWater, void *area);
Vec2i *func_800B50D4(Vec2i *out, u8 dir);
s32 func_800A23E8(Vec2i *from, Vec2i *to);
void *func_800B4E18(Vec2i *position);

static inline void copy_position(Vec2i *out, Vec2i *in) {
    out->x = in->x;
    out->y = in->y;
}

static inline s32 place_failed(void *obj) {
    return func_800AE2A4(obj, 0, 0, &D_801429C0) ^ 1;
}

void func_801EDEF8_13FE18(void *obj, Vec2i *pos) {
    s32 i = 100;

    for (;;) {
        Vec2i here;
        Vec2i target;

        if (--i == -1) {
            break;
        }
        if (place_failed(obj)) {
            return;
        }
        func_800B50D4(&here, 0);
        copy_position(&target, pos);
        if (func_800A23E8(&here, &target) >= 6) {
            return;
        }
        func_800B4E18(&here);
    }
    func_800AE2A4(obj, 0, 0, &D_801429C0);
}
