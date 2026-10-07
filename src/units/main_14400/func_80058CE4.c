#include "field_views_80058CE4.h"

extern u16 D_80163124;
extern u8 D_80163126;
extern u8 D_80163127;
extern u16 D_8016312A;
extern u16 D_8016312C;

void func_80058CE4(u16 *out_24, u16 *out_2a, u16 *out_2c,
                   u8 *out_26, u8 *out_27)
{
    if (out_24 != 0) {
        *out_24 = D_80163124;
    }
    if (out_26 != 0) {
        *out_26 = D_80163126;
    }
    if (out_27 != 0) {
        *out_27 = D_80163127;
    }
    if (out_2a != 0) {
        *out_2a = D_8016312A;
    }
    if (out_2c != 0) {
        *out_2c = D_8016312C;
    }
}
