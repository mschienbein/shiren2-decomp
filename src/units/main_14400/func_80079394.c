#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct Entry {
    u8 pad0[2];
    s16 field_2;
    u8 pad4[0xE];
    u8 field_12;
    u8 field_13;
    u8 pad14[0x23];
    u8 field_37;
    u8 pad38[0xE];
    u8 field_46;
    u8 field_47;
    u8 pad48[0x68];
} Entry;
extern Entry D_801DD378[32];
Entry *func_8007946C(s32 arg0, s32 arg1);

void func_80079394(void) {
    Entry *entry;
    Entry *other;
    Entry *last;

    entry = D_801DD378;
    last = entry + 31;
    for (; entry <= last; entry++) {
        if (entry->field_2 != -1 && entry->field_37 == 1) {
            other = func_8007946C(entry->field_12, entry->field_13);
            if (other != 0 && other->field_2 != -1) {
                entry->field_46 = other->field_46;
                entry->field_47 = other->field_47;
            }
        }
    }
}
