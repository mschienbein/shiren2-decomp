#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

void *func_800C5F60(void);
s32 func_800E1CC4(void *object, s32 kind);
s32 func_80042A68(void)
{
    s32 result = 0;
    if (!((D_80142F18.flags >> 5) & 1)) {
        result = func_800E1CC4(func_800C5F60(), 0) == 0;
    }
    return result;
}
