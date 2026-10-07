#include "common.h"

/* Partial view: only the field read here is known. */
typedef struct {
    unsigned char pad00[0xA];
    unsigned char field0A;
} Record_800A7DD4;

unsigned char func_800A7DD4(Record_800A7DD4 *record)
{
    return record->field0A;
}
