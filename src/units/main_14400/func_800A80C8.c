#include "common.h"

/* Partial view: only the field stored here is known. */
typedef struct {
    unsigned char pad00[0x20];
    u32 field20;
} Record_800A80C8;

void func_800A80C8(Record_800A80C8 *record, u32 value)
{
    record->field20 = value;
}
