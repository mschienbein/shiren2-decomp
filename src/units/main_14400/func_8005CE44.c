#include "common.h"

/* Two 0x50-byte records at D_801DEA0C (0x801DEA0C..0x801DEAAC). func_8005CBE8
 * resets the +0x44 word of both records to -1; this clears one of them.
 * Only +0x44 is accessed here; the rest of the record stays opaque. */
typedef struct {
    char pad0[0x44];
    s32 field_44;
    char pad48[0x8];
} Record_801DEA0C;

extern Record_801DEA0C D_801DEA0C[2];

void func_8005CE44(s32 index) {
    D_801DEA0C[index].field_44 = -1;
}
