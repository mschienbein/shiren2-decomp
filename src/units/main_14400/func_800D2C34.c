#include "common.h"

typedef struct {
    unsigned char pad00[0xC];
    s32 field0C;
    s32 field10;
} Record_800D2C34;

void func_800D2C34(Record_800D2C34 *record, s32 amount)
{
    if (amount >= 0) {
        record->field0C += amount;
    } else {
        record->field10 -= amount;
    }
}
