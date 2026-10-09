#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;



s32 func_800A9BC0(u8 id, u8 *valid) {
    if ((u8)(id - 0x18) < 5) {
        s32 n = id - 0x18;
        s32 bit0 = 1;
        *valid = bit0;
        return !(D_80142F24.masks[0] & (bit0 << n)) && (D_80142F24.masks[1] & (bit0 << n));
    }
    return 0;
}