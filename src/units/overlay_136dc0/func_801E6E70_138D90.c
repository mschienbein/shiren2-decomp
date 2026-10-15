#include "func_801E6E70_138D90/types.h"

void func_801E6E70_138D90(void)
{
    s32 i;
    for (i = 7; i != -1; i--) {
        OvlEffect *p = &D_801F6EAA_148DCA[i];
        if (!p->b02) continue;
        if (p->b03 && (func_8008C1C8(p->b02 - 1, 1.0f) & 1)) {
            if (p->b00 == 0) {
                p->b03 = 0;
                func_8008C194(p->b02 - 1);
                p->b02 = 0;
                if (p->b01 != 255) func_801E6394_1382B4(p->b01, 0);
                p->b01 = 0;
            } else if (p->b00 == 2) {
                func_8008C334(p->b02 - 1);
                func_8008C1C8(p->b02 - 1, 1.0f);
            }
        }
        if (p->b02 && p->b04) {
            HostUnit *unit = func_8007946C(0, p->b01);
            Vec3 position;
            position.x = p->h06 + ((s32)((u32)unit->h0C << 16) >> 18);
            position.y = p->h08 + ((s32)((u32)unit->h0E << 16) >> 18);
            position.z = p->h0A + ((s32)((u32)unit->h10 << 16) >> 18);
            func_8008C588(p->b02 - 1, &position, 1.0f);
        }
    }
}
