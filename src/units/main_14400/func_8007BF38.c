#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 unk0;
    u8 unk1;
    s32 unk4;
    s32 unk8;
} Entry_801A75F0;

extern Entry_801A75F0 D_801A75F0[];

s32 func_8007BF38(s32 arg0) {
    Entry_801A75F0 *entry;

    if (arg0 >= 16) {
        return -1;
    }
    entry = &D_801A75F0[arg0];
    entry->unk0 = 0;
    entry->unk1 = 0;
    entry->unk4 = -1;
    entry->unk8 = -1;
    return arg0;
}
