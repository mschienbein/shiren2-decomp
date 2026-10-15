#include "func_801E5CC0_137BE0/types.h"

D_8007946C_sprite *func_8007946C(s32 kind, s32 handle);
s32 func_800751B4(s32 handle, s32 flags);

/* Interface proposal: alpha is 16-bit, not the inherited s32. Both s16 and
 * u16 match; all four recovered callers pass 0 or 255, so signedness is
 * unresolved. Keep frames s16: callers include an arithmetic shift by 16. */
void func_801E5CC0_137BE0(u8 index, s16 frames, s16 alpha) {
    D_801F6764_view *fade = &D_801F6764_148684[index];
    D_8007946C_sprite *sprite = func_8007946C(0, index);

    if (frames == 0) {
        sprite->alpha = alpha;
        return;
    }
    fade->x0 = frames;
    fade->x3 = frames;
    fade->x2 = ~alpha;
    fade->x1 = alpha;
    fade->x4 = sprite->xB;
    func_800751B4(index, 0x8000);
}
