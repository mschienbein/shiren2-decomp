#include "common.h"

/* Two static 0x50-byte slots (.bss 0x801609D0..0x80160A6F); bit i of
 * D_80138B04 marks slot i as in use (claimed by func_80043690). */
typedef struct { unsigned char data[0x50]; } Slot;

extern Slot D_801609D0[2];
unsigned char D_80138B04 = 0;

/* Releases the slot at the given address. */
void func_800436F0(void *slot)
{
    s32 i;

    for (i = 1; i != -1; i--) {
        if (&D_801609D0[i] == slot) {
            D_80138B04 &= ~(1 << i);
            return;
        }
    }
}
