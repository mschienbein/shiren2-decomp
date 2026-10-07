#include "common.h"
/* Owner field view: both grids and the twelve reset slots at 0xBB. */
typedef struct {
    unsigned char a[5][4];
    unsigned char b[5][4];
    unsigned char unknown28[0xBB - 0x28];
    signed char slots[12];
} Owner;
void func_800D1D54(Owner *owner);
static inline void clear_bytes(unsigned char *d, s32 n) {
    n--;
    do {
        *d++ = 0;
    } while (n-- > 0);
}
void func_800D1830(Owner *g) {
    s32 i = 0;
    for (;;) {
        s32 j;
        if (i >= 5) break;
        j = 0;
        for (;;) {
            s32 size = 1;
            if (j >= 4) break;
            g->a[i][j] = 0;
            clear_bytes(&g->b[i][j], size);
            j++;
        }
        i++;
    }
    func_800D1D54(g);
}
