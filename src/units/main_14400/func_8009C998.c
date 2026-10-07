#include "common.h"

/* Partial view: only the field stored here is known. */
typedef struct {
    unsigned char pad00[0x58];
    u32 field58;
} Record_8009C998;

void func_8009C998(Record_8009C998 *record, u32 value)
{
    record->field58 = value;
}
