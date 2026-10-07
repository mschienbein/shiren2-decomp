#include "common.h"

/* Partial view: only the field stored here is known. */
typedef struct {
    unsigned char pad00[0x48];
    u32 field48;
} Record_800960D0;

void func_800960D0(Record_800960D0 *record, u32 value)
{
    record->field48 = value;
}
