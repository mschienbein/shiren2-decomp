#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;

typedef signed char s8;



extern s32 func_800AA03C(void);
extern void func_800A9C8C(u8 arg0, u8 arg1);

s8 func_800AA0BC(void) {
    s8 step;
    u8 *cursor;

    s32 notReady = func_800AA03C() != 1;

    if (notReady) {
        return 0;
    }
    switch (D_80142F18.flags & 3) {
        case 1:
            step = 1;
            break;
        case 2:
            step = -1;
            break;
        default:
            return 0;
    }
    /* Character pointer ranges over the complete eleven-byte object. */
    cursor = (u8 *)&D_80142F24 + 1;
    *cursor += step;
    func_800A9C8C(cursor[-1], *cursor);
    return step;
}
