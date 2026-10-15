#include "func_801ED5A0_13F4C0/types.h"

extern u8 D_801F5CCD_147BED;
extern u8 D_801F5228_147148[32];
extern u8 D_80147620[];
u8 func_801E8CF4_13AC14(u8, u8);
s32 func_800C5844(void *, u8, u8);
void func_801EB6A0_13D5C0(u8);
s32 func_801EA124_13C044(s32, s32);
void func_801EC3D0_13E2F0(void);
void func_801E85E0_13A500(void);
s32 func_800327C0(char *dst, const char *fmt, ...);
s32 func_801ECF5C_13EE7C(const char *text, s32 index);
void func_801EE8CC_1407EC(s32);

u16 func_801ED5A0_13F4C0(void) {
    char text[16];
    s32 i;

    switch (D_801F5CCD_147BED) {
    case 0x28:
        func_801E8CF4_13AC14(D_801F51E8_147108->x108, D_801F51E8_147108->x10C);
        break;
    case 0x29:
        func_801E8CF4_13AC14(D_801F51E8_147108->x108, func_800C5844(D_80147620, 0, D_801F51E8_147108->x10C));
        break;
    case 0x2A:
        func_801EB6A0_13D5C0(1);
        func_801E8CF4_13AC14(D_801F51E8_147108->x108, D_801F5228_147148[D_801F51E8_147108->x108 & 0xFF] + D_801F51E8_147108->x10C);
        break;
    case 0x2B:
        func_801EB6A0_13D5C0(1);
        func_801EA124_13C044(D_801F51E8_147108->x108, D_801F51E8_147108->x10C);
        break;
    case 0x2C:
        func_801EC3D0_13E2F0();
        break;
    case 0x2D:
        func_801E85E0_13A500();
        break;
    case 0x2E:
        /* ODD_C: recognized success/no-action opcode, not the default failure.
         * Entry 6 of jtbl_801F5F78_147E98 targets 801ED70C (return 1), whereas default
         * 801ED704/708 returns 0. Deleting this arm changes semantics.
         * Equivalent explicit return 1 is retained in alternatives/ for the
         * final measurement; see final-results.json for that receipt. */
        break;
    case 0x2F:
        func_800327C0(text, "%d", D_801F5228_147148[D_801F51E8_147108->x108 & 0xFF]);
        for (i = 0; i < 3; i++) {
            func_801ECF5C_13EE7C(text, i);
        }
        func_801EE8CC_1407EC(1);
        break;
    default:
        return 0;
    }
    return 1;
}
