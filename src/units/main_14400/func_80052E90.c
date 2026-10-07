#include "common.h"

typedef struct {
    s32 unk0;
    unsigned char unk4;
    unsigned char unk5;
    unsigned char pad6[6];
} Entry;

extern Entry D_801397E4[];

void func_80052E90(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        Entry *entry = &D_801397E4[i];

        entry->unk4 = 0;
        entry->unk5 = 0x80;
        entry->unk0 = 0;
    }
}
