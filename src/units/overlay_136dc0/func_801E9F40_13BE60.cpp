#include "func_801E9F40_13BE60/types.h"

extern "C" s32 func_801E9F40_13BE60(u8 *text) {
    while (*text != 0) {
        if (*text++ == '\n') {
            return 1;
        }
    }
    return 0;
}
