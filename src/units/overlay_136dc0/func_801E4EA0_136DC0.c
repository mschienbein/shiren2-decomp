#include "func_801E4EA0_136DC0/types.h"

void func_801EC510_13E430(void);
void func_801EC4D0_13E3F0(void);
void func_801EC54C_13E46C(void);
void func_801EA0F4_13C014(void);
void func_801EA0A4_13BFC4(void);
void func_80048380(void);
void func_80048334(s32 mode, s32 unused_message, ...);

s32 func_801E4EA0_136DC0(void) {
    D_801F51E8_view *state = D_801F51E8_147108;
    s32 mode = 0;
    u8 index;
    u8 flags;

    if (state != 0) {
        if (state->x9F1 == 0) {
            return 0;
        }
        index = state->x9F2;
        flags = state->x8F0[index];
        if (flags & 0x10) {
            func_801EC510_13E430();
            D_801F51E8_147108->x9F2++;
            func_80048380();
        } else if (flags & 4) {
            func_801EC510_13E430();
            if (D_801F51E8_147108->x2B0[D_801F51E8_147108->x9F2][0] == 0) {
                D_801F51E8_147108->x9F2++;
            } else {
                D_801F51E8_147108->x2B0[D_801F51E8_147108->x9F2][0]--;
            }
        } else if (flags & 8) {
            func_801EA0F4_13C014();
            func_801EA0A4_13BFC4();
        } else {
            if (flags & 1) {
                mode = 1;
            }
            /* The host uses a fixed "%s"; the message argument is unused. */
            func_80048334(mode, 0x4F9, state->x2B0[index]);
            if (D_801F51E8_147108->x8F0[D_801F51E8_147108->x9F2] & 2) {
                func_801EC4D0_13E3F0();
            }
            D_801F51E8_147108->x9F2++;
        }
        if (D_801F51E8_147108->x9F2 == D_801F51E8_147108->x9F1) {
            func_801EC54C_13E46C();
            func_801EC510_13E430();
        }
        return 1;

    }
    return 0;
}
