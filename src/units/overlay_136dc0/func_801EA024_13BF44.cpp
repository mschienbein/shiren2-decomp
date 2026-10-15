#include "func_801EA024_13BF44/types.h"

extern "C" s32 func_801EA024_13BF44(void) {
    s32 result = 1;
    if (D_801F5D04_147C24 == 1) {
        if (D_801F51E8_147108->unk_996) {
            func_801E8F20_13AE40(20);
        } else {
            func_801E7C6C_139B8C(1);
            do {
                func_801E8F20_13AE40(1);
                result = func_801E51A0_1370C0();
            } while (result == 0);
            func_801E7C6C_139B8C(0);
        }
    }
    return result;
}
