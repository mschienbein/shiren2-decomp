#include "func_801E9B64_13BA84/types.h"

extern "C" void func_801E9B64_13BA84(s16 count) {
    s16 slot;
    s16 room;
    for (; count > 0; count--) {
        s16 best = 0;
        s16 bestSlot = -1;
        for (slot = 4; slot != -1; slot--) {
            room = func_800D1988(func_800C9E00(), slot) - func_801E99C4_13B8E4(slot, 0);
            if (room > best) {
                best = room;
                bestSlot = slot;
            }
        }
        if (bestSlot >= 0) {
            u8 index = bestSlot;
            u8 level = func_801E99C4_13B8E4(index, 0) + 1;
            func_801E9AFC_13BA1C(index, level);
            func_801E8CF4_13AC14(7, index);
            func_801E8CF4_13AC14(6, level);
        }
    }
}
