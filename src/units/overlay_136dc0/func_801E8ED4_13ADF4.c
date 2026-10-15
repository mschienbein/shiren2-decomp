#include "func_801E8ED4_13ADF4/types.h"
extern s32 D_801F5D04_147C24;
extern s32 func_801E54B8_1373D8(s32 count, s32 mode);
void func_801E8ED4_13ADF4(s32 count) {
    s32 mode = D_801F51E8_147108->field120 != 0;
    if (D_801F51E8_147108->field120 & 0x8000) mode = 2;
    if (D_801F5D04_147C24 == 1) func_801E54B8_1373D8(count, mode);
}
