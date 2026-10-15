#include "func_801E4F94_133AE4/types.h"

void func_801E54A4_133FF4(s32 index, s32 mode);
s32 func_801E59A4_1344F4(void);

s32 func_801E4F94_133AE4(s32 slot, s32 mode) {
    if (D_801E7830_136380[slot] == 0xFF) {
        return -1;
    }
    func_801E54A4_133FF4(D_801E7830_136380[slot], mode + 1);
    return func_801E59A4_1344F4();
}
