#include "common.h"

typedef struct {
    unsigned char pad_00[0x0C];
    unsigned char flags_0C;
} FlagRecord;

s32 func_80128B24(FlagRecord *record)
{
    unsigned char flags = record->flags_0C & 4;
    return flags != 0;
}
