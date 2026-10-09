#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0xA]; u8 field_0A; u8 pad_0B[0x13]; u8 field_1E; u8 pad_1F[0x13]; u8 field_32; u8 pad_33[0xB1]; } Record;
extern Record D_801C36EC[30]; /* Complete table; the scan visits the first 29. */
extern u8 D_801C51A4[];
extern const unsigned char D_8015488C[8];

static inline s32 has_value(Record *record, u8 value) {
    s32 same = 0;
    if (record->field_1E & 0x7C) {
        same = value == record->field_32;
    }
    return same;
}

static inline s32 bit_at(u8 *bits, s32 index) {
    s32 result = 0;
    if (bits[index >> 3] & D_8015488C[index & 7]) {
        result = 1;
    }
    return result;
}

s32 func_800A8AA0(s32 id, s32 arg) {
    s32 count = 0;
    s32 index = 0;
    u8 value = arg;
    for (;;) {
        s32 used;
        Record *record;
        if (index >= 29) break;
        record = &D_801C36EC[index];
        used = bit_at(D_801C51A4, index);
        if (used) {
            if (record->field_0A == id) {
                if (value == 0) {
                    count++;
                } else {
                    s32 same = has_value(record, value);
                    if (same) {
                        count++;
                    }
                }
            }
        }
        index++;
    }
    return count;
}
