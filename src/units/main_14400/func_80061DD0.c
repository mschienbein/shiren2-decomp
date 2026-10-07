#include "common.h"

extern u32 D_801D9358[][54];
extern s32 D_8016DB18;
extern s32 D_8013B800;
extern void func_80041814(void *, s32, s32, s32, s32);
extern void func_8007D8C8(void);
extern void func_8007F090(s32, s32);
extern void func_80062430(s32, s32, s32, s32);

void func_80061DD0(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x, y;
    if (x0 < 10) x0 = 10;
    else if (x0 > 65) x0 = 65;
    if (y0 < 10) y0 = 10;
    else if (y0 > 43) y0 = 43;
    if (x1 < 10) x1 = 10;
    else if (x1 > 65) x1 = 65;
    if (y1 < 10) y1 = 10;
    else if (y1 > 43) y1 = 43;
    func_80041814(D_801D9358, x0, y0, x1, y1);
    func_8007D8C8();
    for (x = x0 - 1; x <= x1 + 1; x++) {
        u32 *cell = &D_801D9358[x][y0 - 1];
        for (y = y0 - 1; y <= y1 + 1; y++, cell++) {
            *cell |= 0x800000;
            if (*cell & 0x400) {
                func_8007F090(x, y);
            }
        }
    }
    if (D_8016DB18 == 3 || D_8013B800) {
        D_8013B800 = 0;
        func_80062430(x0 - 10, y0 - 10, x1 - 10, y1 - 10);
    }
}
