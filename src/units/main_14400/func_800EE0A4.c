#include "common.h"

typedef struct {
    unsigned char pad_00[0xE4];
    unsigned short flags_E4;
} FlagRecord;

void func_800EE0A4(FlagRecord *record)
{
    record->flags_E4 |= 0x100;
}
