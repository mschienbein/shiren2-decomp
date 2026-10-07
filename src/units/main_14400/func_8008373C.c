#include "row_views.h"

s32 func_8008373C(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].field_02 != 0) {
            return 0x40;
        }
    }
    return 0;
}
