#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

/* Measured field view only, not a recovered owning object or original name. */
typedef struct Func80056690FieldView {
    u8 unknown00[0x14];
    u8 field14;
    u8 field15;
    u8 field16;
    u8 field17;
    u8 field18;
    u8 field19;
    u8 field1A;
    u8 field1B;
    u8 unknown1C[0x18];
    float field34;
    float field38;
    u8 unknown3C[0x14];
    s16 field50;
    u8 unknown52[0x8];
    s16 field5A;
    s16 field5C;
    s16 field5E;
} Func80056690FieldView;

typedef char view_has_60_bytes[(sizeof(Func80056690FieldView) == 0x60) ? 1 : -1];
typedef char view_float_is_32_bits[(sizeof(float) == 4) ? 1 : -1];
typedef char view_halfword_is_16_bits[(sizeof(s16) == 2) ? 1 : -1];

void func_80056690(Func80056690FieldView *record)
{
    record->field14 = 0xFF;
    record->field15 = 0xFF;
    record->field16 = 0xFF;
    record->field18 = 0xFF;
    record->field19 = 0xFF;
    record->field1A = 0xFF;
    record->field1B = 0xFF;
    record->field17 = 0;
    record->field5E = 1;
    record->field5C = 0;
    record->field50 = 1;
    record->field5A = 0;
    record->field34 = 1.1f;
    record->field38 = 1.1f;
}
