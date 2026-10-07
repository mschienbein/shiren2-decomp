#include "common.h"

/* Partial view: only the field stored here is known. */
typedef struct {
    unsigned char pad00[0x45];
    unsigned char field45;
} Record_800957AC;

void func_800957AC(Record_800957AC *record, unsigned char value)
{
    record->field45 = value;
}
