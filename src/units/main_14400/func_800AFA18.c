#include "common.h"

typedef unsigned char u8;
typedef struct { s32 field_00; u8 *field_04; s32 field_08; s32 field_0C; } Bits;
extern void *D_80143104;
extern const unsigned char D_8015488C[8];

static inline s32 bit_at(u8 *bits, s32 index) {
    s32 result = 0;
    if (bits[index >> 3] & D_8015488C[index & 7]) {
        result = 1;
    }
    return result;
}

s32 func_800AFA18(Bits *bits) {
    s32 index;
    s32 end;
    if (D_80143104) {
        return 1;
    }
    index = bits->field_0C;
    end = bits->field_08;
    for (;;) {
        s32 set;
        if (index >= end) break;
        set = bit_at(bits->field_04, index);
        if (set == 0) {
            return 1;
        }
        index++;
    }
    return 0;
}
