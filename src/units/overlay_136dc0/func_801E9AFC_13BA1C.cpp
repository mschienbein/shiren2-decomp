#include "func_801E9AFC_13BA1C/types.h"

extern "C" void func_801E9AFC_13BA1C(u8 slot, u8 value) {
    s16 bit = slot * 3 + 200;
    func_801E9F90_13BEB0(bit++, value & 1);
    func_801E9F90_13BEB0(bit++, (value >> 1) & 1);
    func_801E9F90_13BEB0(bit, (value >> 2) & 1);
}
