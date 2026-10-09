#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;


void func_800AA3C0(void) {
    u8 *flags = &D_80142F18.flags;

    *flags |= 0x40;
}
