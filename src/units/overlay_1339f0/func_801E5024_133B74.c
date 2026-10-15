#include "func_801E5024_133B74/types.h"

void func_801E55DC_13412C(s32 index);
s32 func_801E59A4_1344F4(void);

s32 func_801E5024_133B74(s32 slot) {
    if (D_801E7830_136380[slot] == 0xFF) {
        return -1;
    }
    func_801E55DC_13412C(D_801E7830_136380[slot]);
    return func_801E59A4_1344F4();
}
