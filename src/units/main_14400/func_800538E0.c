#include "common.h"

/* The sole 0x420-byte record spans D_80161724 through 0x80161B43. */
typedef struct { s32 field_0, field_4, field_8, field_C, field_10, field_14, field_18, field_1C; char messages_20[4][0x100]; } Slot;
extern Slot D_80161724[1];
extern void func_80054AC4(void);

void func_800538E0(void)
{
    Slot *record = &D_80161724[1];
    s32 index = 0;
    do {
        --record;
        record->field_0 = -1;
    } while (--index != -1);
    func_80054AC4();
}
