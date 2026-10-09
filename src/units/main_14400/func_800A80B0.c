#include "common.h"

typedef struct {
    unsigned char pad_00[0x20];
    u32 flags_20;
} FlagRecord;

void func_800A80B0(FlagRecord *record, u32 flags)
{
    record->flags_20 |= flags;
}
