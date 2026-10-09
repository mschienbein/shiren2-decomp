#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;

s32 func_80041E3C(void) { return (D_80142F18.flags >> 2) & 1; }
