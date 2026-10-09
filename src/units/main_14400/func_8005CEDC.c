#include "common.h"
typedef struct {
    short field_00, field_02;
    unsigned char field_04[10]; unsigned char field_0e, field_0f;
    unsigned char field_10, field_11, field_12, field_13;
    unsigned char field_14[2]; unsigned char field_16, field_17, field_18, field_19, padding_1A[6];
} Settings;
typedef struct {
    unsigned char field_00[0x14]; s32 field_14;
    unsigned char field_18[0x1c]; float field_34, field_38;
    unsigned char field_3c[0x14];
} Output;
extern Settings D_80165960;
extern Output D_801DEA0C[2];
void func_8005CEDC(void) {
    Settings *settings = &D_80165960;
    Output *first = D_801DEA0C;
    Output *second = first + 1;
    float value = settings->field_00;
    s32 left, right;
    value *= 0.03125;
    if (settings->field_0f != 0)
        value = value * (float)settings->field_0e / (float)settings->field_0f;
    first->field_34 = value;
    second->field_34 = value;
    value = settings->field_02;
    if (settings->field_11 != 0)
        value = value * (float)settings->field_10 / (float)settings->field_11;
    value = value * 0.03125;
    first->field_38 = value;
    second->field_38 = value;
    if (settings->field_18 != settings->field_19) {
        value = 512.0f;
        if (settings->field_16 != 0) {
            s32 divisor = settings->field_16;
            value = settings->field_17 - (settings->field_16 >> 1);
            if (value < 0.0f) value = -value;
            value = 2.0f * (value * 512.0f / (float)divisor);
        }
        right = (s32)value;
        left = 0x200 - right;
        if (left >= 0x100) left = 0xff;
        first->field_14 = left;
        left = right;
        if (left >= 0x100) left = 0xff;
        second->field_14 = left;
    } else if (settings->field_12 != settings->field_13) {
        value = settings->field_12 * 0xff / settings->field_13;
        left = (s32)value;
        first->field_14 = left;
        second->field_14 = left;
    } else {
        first->field_14 = 0xff;
        second->field_14 = 0xff;
    }
}
