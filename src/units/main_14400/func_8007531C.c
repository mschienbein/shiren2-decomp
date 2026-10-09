#include "common.h"
typedef struct {
    short field_00; unsigned char field_02[5]; unsigned char field_07;
    unsigned char field_08[4]; short field_0c, field_0e;
    unsigned char field_10[2]; unsigned char field_12, field_13;
    unsigned char field_14[8]; float field_1c, field_20;
    unsigned char field_24[0x10]; unsigned char field_34;
    unsigned char field_35[2]; unsigned char field_37;
    unsigned char field_38[0xc]; unsigned char field_44, field_45, field_46, field_47;
    unsigned char field_48[0x68];
} Entry;
extern Entry D_801D2C2C[], D_801DEAB4[];
extern s32 func_80074500(Entry *, s32, s32, s32, s32);
/* x/y arrive full width from every caller (mfc1 of trunc.w.s in func_80075410, e.g. 0x8007596C/
 * 0x80075974); the halfword stores to field_0c/field_0e narrow them. */
s32 func_8007531C(s32 index, s32 value, s32 x, s32 y, float first, float second) {
    Entry *source = &D_801DEAB4[index];
    Entry *dest = &D_801D2C2C[index];
    s32 result = func_80074500(D_801D2C2C, index, 0, 0, value);
    if (result == -1) return -1;
    else {
        dest->field_44 = 0xff;
        dest->field_00 = 5;
        dest->field_37 = 1;
        dest->field_12 = 0;
        dest->field_13 = index;
        dest->field_34 = 2;
        dest->field_07 = 1;
        dest->field_1c = first;
        dest->field_20 = second;
        dest->field_0c = x;
        dest->field_0e = y;
        dest->field_46 = source->field_46;
        dest->field_47 = source->field_47;
        return result;
    }
}
