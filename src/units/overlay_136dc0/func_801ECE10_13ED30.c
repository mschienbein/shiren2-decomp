#include "func_801ECE10_13ED30/types.h"

extern SelectionRecord D_80142F18;
void func_800B75E8(Vec2i *pos);
void func_801F4FE0_146F00(s32);
void func_800265E0(void *dst, s32 length);
Popup801F1510 *func_801F1510_143430(Spec801ECE10 *spec, s32, s32);
void func_801F1B58_143A78(Popup801F1510 *popup, Spec801ECE10 *spec);

static inline s32 controls_disabled(SelectionRecord *record) {
    return (record->flags >> 2) & 1;
}

static inline s32 controls_enabled(SelectionRecord *record) {
    return controls_disabled(record) ^ 1;
}

void func_801ECE10_13ED30(u8 mode, Vec2i *pos) {
    Spec801ECE10 spec;
    Popup801F1510 *popup;

    if (mode & 2) {
        func_800B75E8(pos);
        func_801F4FE0_146F00(controls_enabled(&D_80142F18));
    }
    if (mode & 1) {
        func_800265E0(&spec, 0x16);
        spec.kind_00 = 0x5F;
        spec.coordinates.bytes.y = pos->y;
        spec.coordinates.bytes.x = pos->x;
        spec.h0E = 0x91;
        spec.h0C = 0x297;
        popup = func_801F1510_143430(&spec, 1, 0);
        if (popup != 0) {
            func_801F1B58_143A78(popup, &spec);
            popup->b52 = 1;
        }
    }
}
