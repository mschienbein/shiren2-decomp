#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;



/* Returns the zero-extended selection index as a full int: the only original caller,
 * func_8007D32C, stores v0 un-narrowed as a word to D_8013DEA0 (0x8007D344) and compares
 * that word with slti 0x15 (0x8007D368). */
s32 func_8004246C(void) {
    return D_80142F24.index;
}
