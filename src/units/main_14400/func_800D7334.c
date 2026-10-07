#include "common.h"
extern s32 D_80148090;
s32 func_800D5A68(void);
s32 func_800D6D78(void);
s32 func_800D7334(void) {
    s32 result = 1;
    if (D_80148090 == 1) {
        result = func_800D5A68();
    } else if (D_80148090 > 0) {
        if (D_80148090 < 4) result = func_800D6D78();
    }
    if (result != 2) D_80148090 = 0;
    return result;
}
