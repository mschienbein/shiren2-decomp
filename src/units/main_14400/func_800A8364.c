#include "common.h"

/* Partial view: only the field cleared here is known. */
typedef struct {
    unsigned char pad00[0x1C];
    unsigned short field1C;
} Record_800A8364;

void func_800A8364(Record_800A8364 *record)
{
    record->field1C = 0;
}
