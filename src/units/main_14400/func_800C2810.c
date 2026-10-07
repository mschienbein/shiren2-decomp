#include "common.h"

/* Partial write view; historical record ownership and API remain unresolved. */
typedef struct {
    unsigned char unknown_00[0x0A];
    unsigned char field_0A;
    unsigned char unknown_0B;
    unsigned short field_0C;
    unsigned short field_0E;
    u32 field_10;
    unsigned short field_14;
} RecordView_800C2810;

/* The observed callers use the reset effects and ignore the returned register. */
void func_800C2810(RecordView_800C2810 *record)
{
    record->field_14 = 0;
    record->field_0E = 0;
    record->field_0C = 0;
    record->field_0A = 0;
    record->field_10 = 1;
}
