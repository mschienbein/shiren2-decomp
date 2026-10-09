#include "field_views_80058CE4.h"

typedef struct { u16 field_00; u8 x_02, y_03; u16 field_04, pressed_06, repeat_08, accumulated_0A, previous_0C; u8 delay_0E; } Input;
extern Input D_80163124;

void func_80058CE4(u16 *out_24, u16 *out_2a, u16 *out_2c,
                   u8 *out_26, u8 *out_27)
{
    if (out_24 != 0) {
        *out_24 = D_80163124.field_00;
    }
    if (out_26 != 0) {
        *out_26 = D_80163124.x_02;
    }
    if (out_27 != 0) {
        *out_27 = D_80163124.y_03;
    }
    if (out_2a != 0) {
        *out_2a = D_80163124.pressed_06;
    }
    if (out_2c != 0) {
        *out_2c = D_80163124.repeat_08;
    }
}
