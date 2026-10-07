#include "common.h"

/* Partial view: only the field read here is known. */
typedef struct {
    unsigned char pad00[0x5];
    unsigned char field05;
} Record_800535C8;

unsigned char func_800535C8(Record_800535C8 *record)
{
    return record->field05;
}
