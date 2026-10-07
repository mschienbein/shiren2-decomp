#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 flags;
    u8 pad4[0xC];
    s32 unk10;
    u8 pad14[0x8];
    s32 unk1C;
    u8 pad20[0x24];
    s32 unk44;
    u8 pad48[0x13C - 0x48];
} Entry_8012A434;

extern s32 D_801CA6D4;
extern Entry_8012A434 *D_801CA6DC;

s32 func_8012A434(s32 id, s32 arg1) {
    Entry_8012A434 *entry;
    s32 value;
    s32 i;
    s32 count;

    if (id == 0) {
        return 0;
    }
    value = arg1;
    if (value == 0) {
        value = 1;
    }
    count = 0;
    entry = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, entry++) {
        if (entry->unk44 == id && entry->unk10 == -1) {
            if (entry->flags & 1) {
                entry->unk1C = 1;
                entry->unk10 = 0;
                entry->flags &= ~1;
            } else {
                entry->unk1C = value;
                entry->unk10 = arg1;
            }
            count++;
        }
    }
    return count;
}
