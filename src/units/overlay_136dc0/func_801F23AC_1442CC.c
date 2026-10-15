#include "func_801F23AC_1442CC/types.h"

s32 func_801F23AC_1442CC(u16 id) {
    u16 ids[3] = { 0x4DC, 0x4ED, 0x4F9 };
    s32 i;

    for (i = 2; i != -1; i--) {
        if (id == ids[i]) {
            return 1;
        }
    }
    return 0;
}
