#include "common.h"

extern unsigned short D_8015691C;
extern unsigned short D_8015698C;

/* Entity-family slot +0x54 (D_801535E8+0x54); the receiver supplied by the call contract is unused. */
s32 func_800A6F58(void *self, s32 mode) {
    switch (mode) {
    case 4:
        return D_8015698C;
    case 1:
        return 100 - D_8015691C;
    }
    return 0;
}
