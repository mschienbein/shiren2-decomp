#include "common.h"

typedef struct {
    unsigned char pad00[0x9A];
    unsigned short flags9A;
    unsigned char field9C;
} Record_800F0384;

s32 func_800F0384(Record_800F0384 *record)
{
    s32 result = 0;

    if (record->flags9A & 0x40) {
        result = record->field9C == 0xFF;
    }
    return result;
}
