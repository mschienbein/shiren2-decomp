#include "common.h"

/* Prefix of the channel record shared by the bank and fallback pointers. */
typedef struct Record_8012A7C4 {
    unsigned char pad00[0x10];
    s32 field10;
} Record_8012A7C4;

extern Record_8012A7C4 *D_801CA6FC;

void func_8012A7C4(Record_8012A7C4 *record)
{
    if (record != 0 && record->field10 < 0) {
        D_801CA6FC = record;
    }
}
