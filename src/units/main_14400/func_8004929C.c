#include "common.h"
extern unsigned char D_80139610, D_80139638[];
s32 func_8004929C(void) {
    s32 count = D_80139610;
    unsigned char *cursor = D_80139638;
    while (--count != -1) {
        while (*cursor++) {}
    }
    return cursor - D_80139638;
}
