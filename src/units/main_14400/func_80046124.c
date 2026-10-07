#include "common.h"
extern unsigned char D_80142F18;
extern unsigned char D_80138BE0[];
extern unsigned char D_80138BE8[];
extern char D_80147600[];
unsigned char func_800C57CC(void *rng, unsigned char max);
s32 func_80046124(void) {
    unsigned char *table;
    unsigned char n;
    if (D_80142F18 < 20) {
        table = D_80138BE0;
        n = 8;
    } else {
        n = 7;
        table = D_80138BE8;
    }
    return table[func_800C57CC(D_80147600, n - 1)];
}
