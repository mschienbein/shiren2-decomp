#include "common.h"

/* Partial view: only the field read here is known. */
typedef struct {
    unsigned char pad00[0x124];
    u32 field124;
} Record_8009FAF8;

u32 func_8009FAF8(Record_8009FAF8 *record)
{
    return record->field124;
}
