#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u16 field_00; u16 field_02; u8 field_04[0x16]; u16 field_1A; u8 field_1C[0x10]; } Entry;
extern Entry D_801A9080[10];
s32 func_8008269C(s32 kind) {
    s32 found = 0;
    s32 i;
    Entry *entries = D_801A9080;
    for (i = 0; i < 10; i++) {
        if (entries[i].field_02 != 0 && entries[i].field_02 != 0x41 && entries[i].field_1A == (u16)kind) {
            found++;
            break;
        }
    }
    if (!found) return -1;
    return i;
}
