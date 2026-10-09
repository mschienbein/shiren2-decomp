#include "common.h"

typedef unsigned char u8;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

s32 func_80041C44(void) {
    return D_80142F18.kind;
}
