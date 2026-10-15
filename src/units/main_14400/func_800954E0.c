#include "common.h"

typedef struct {
    unsigned char pad00[0x20];
    s32 field20;
    unsigned char pad24[4];
    u32 field28;
    u32 field2C;
    unsigned short field30;
    unsigned char pad32[0x14];
    signed char field46;
} Record_800954E0;

typedef struct {
    s32 field00;
    /* func_800982DC supplies a full word at +4, narrowed by this setter. */
    s32 field04;
    u32 field08;
    u32 field0C;
} Params_800954E0;

void func_80095D20(Record_800954E0 *record);
void func_80046E7C(Record_800954E0 *record);

void func_800954E0(Record_800954E0 *record, Params_800954E0 *params)
{
    s32 active = record->field46 != 0;
    s32 value;

    if (!active) {
        func_80095D20(record);
    }
    value = params->field00;
    record->field20 = value;
    if (value <= 0) {
        record->field20 = 1;
    }
    record->field30 = (unsigned short)params->field04;
    record->field28 = params->field08;
    record->field2C = params->field0C;
    if (!active) {
        func_80046E7C(record);
    }
}
