#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;

u8 *func_800C5F60(void);
s32 func_80049CB4(s32, ...);

s32 func_800DF910(void *self /* receiver: unused; supplied by the vtable +0x14 call */)
{
    u8 *p = func_800C5F60();

    if (!((D_80142F18.flags >> 2) & 1)) {
        if (!(p[0x72] & 0x10)) {
            p[0x72] |= 0x10;
            func_80049CB4(0x1F, p);
        }
    }
    return 0;
}
