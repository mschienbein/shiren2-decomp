#include "common.h"

typedef unsigned char u8;

extern u8 D_80139610;
extern u8 D_80139638[0x50];

/* Drop the oldest NUL-terminated entry from the 0x50-byte queue and decrement its count. */
void func_80049110(void) {
    s32 i;
    s32 j;
    s32 k;

    if (D_80139610 == 0) {
        return;
    }
    if (D_80139610 == 1) {
        k = 0x4F;
        while (1) {
            D_80139638[k] = 0;
            if (--k == -1) {
                break;
            }
        }
    } else {
        i = 0;
        while (D_80139638[i++] != 0) {
        }
        for (j = 0; i < 0x50; i++, j++) {
            D_80139638[j] = D_80139638[i];
        }
    }
    D_80139610--;
}
