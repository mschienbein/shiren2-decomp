#include "common.h"
typedef unsigned char u8;
typedef struct { unsigned short first_00; u8 count_02; u8 field_03; } Range;
extern u8 D_80160B58[0x18];
extern u8 D_00194FC0[];
extern u8 D_2025D48[];
Range *func_80045134(u8 kind);
void func_8006AC30(void *dst, void *romBase, void *segAddr, s32 stride, s32 first, s32 count);
u8 *func_800451A8(u8 kind, u8 variant)
{
    Range *range = func_80045134(kind);
    s32 index;
    if (kind == 0x5B) {
        index = range->first_00;
    } else {
        if (range->count_02 < variant) {
            return 0;
        }
        index = range->first_00;
        --index;
        index += variant;
    }
    func_8006AC30(D_80160B58, D_00194FC0, D_2025D48, 0x18, index, 1);
    return D_80160B58;
}
