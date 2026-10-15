#include "func_801E4F20_133A70/types.h"

s32 func_801E53BC_133F0C(s32 index);
s32 func_801E59A4_1344F4(void);
u32 func_801E5788_1342D8(void);
u8 *func_801E57A4_1342F4(void);

u8 *func_801E4F20_133A70(s32 slot, s32 *message) {
    *message = -1;
    if (D_801E7830_136380[slot] == 0xFF) {
        return 0;
    }
    func_801E53BC_133F0C(D_801E7830_136380[slot]);
    *message = func_801E59A4_1344F4();
    if (func_801E5788_1342D8() != 0) {
        return 0;
    }
    return func_801E57A4_1342F4();
}
