#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

extern unsigned char D_80138BE0[];
extern unsigned char D_80138BE8[];
extern char D_80147600[];
unsigned char func_800C57CC(void *rng, s32 max);
s32 func_80046124(void) {
    unsigned char *table;
    unsigned char n;
    if (D_80142F18.kind < 20) {
        table = D_80138BE0;
        n = 8;
    } else {
        n = 7;
        table = D_80138BE8;
    }
    return table[func_800C57CC(D_80147600, (unsigned char)(n - 1))];
}
