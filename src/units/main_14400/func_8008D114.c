#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00;
    u8 field_01[2];
    u8 field_03;
    unsigned short field_04;
    unsigned short field_06;
    u8 field_08[8];
    u8 field_10[0x10];
} Entry;
extern u8 func_8008D758(void *, u8);

s32 func_8008D114(Entry *entries, s32 key, s32 value) {
    u32 index;
    for (index = 0; index < 8; index++) {
        Entry *entry = &entries[index];
        if (entry->field_00 == 1 && entry->field_06 == key) {
            u8 result = func_8008D758(entry->field_10, value);
            if (entry->field_03 == result) {
                return index;
            }
        }
    }
    return -1;
}
