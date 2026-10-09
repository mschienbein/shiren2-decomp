#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;


s32 func_800AA24C(void);
s32 func_800AA184(u8 a, u8 b);
void func_800A9EB8(void);

void func_800AA144(void) {
    s32 failed = func_800AA184(func_800AA24C(), D_80142F18.kind) != 1;
    if (failed) {
        func_800A9EB8();
    }
}
