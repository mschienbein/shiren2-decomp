#include "func_801E5D6C_137C8C/types.h"

D_8007946C_sprite *func_8007946C(s32 kind, s32 handle);
s32 func_800751B4(s32 handle, s32 flags);
void func_801E7A28_139948(s32 index);
void func_801E6394_1382B4(s32 arg0, s32 arg1);
void func_800420E8(s32 id);

void func_801E5D6C_137C8C(void) {
    s32 i;
    D_801F6764_view *fade;
    D_8007946C_sprite *sprite;

    i = 30;
    while (--i != -1) {
        fade = &D_801F6764_148684[i];
        if (fade->x0 == 0) {
            continue;
        }
        sprite = func_8007946C(0, i);
        if (--fade->x0 == 0) {
            sprite->alpha = fade->x1;
            func_800751B4(i, fade->x4);
            /* ODD_C: x0 is already 0 here; the ROM still stores it (801E5DDC sb zero,0(s0)).
             * Removing this store gives 2 differing words under the s32 contract
             * (independently measured by Opus: host-contracts-opus/edge/m_nostore). */
            fade->x0 = 0;
            if (fade->x1 == 0) {
                func_801E7A28_139948(i);
                if (i == 0) {
                    func_801E6394_1382B4(0, 1);
                } else {
                    func_800420E8(i);
                }
            }
        } else {
            sprite->alpha = fade->x2 + (fade->x1 - fade->x2) * (fade->x3 - fade->x0) / fade->x3;
        }
    }
}
