#include "common.h"

extern u32 D_801CA95C;
s32 D_80148AC0 = 1;
extern s32 D_801CA950;
extern s32 D_801CA954;
extern s32 D_801CA958;

s32 func_8012D9DC(u32 addr) {
    u32 base = D_801CA95C;

    if (base + 0xB8 < addr) {
        if (D_80148AC0 != 0) {
            D_80148AC0 = 0;
            return D_801CA954;
        }
    } else if (addr < base) {
        if (D_80148AC0 != 0) {
            D_80148AC0 = 0;
            return D_801CA958;
        }
    } else {
        D_80148AC0 = 1;
    }
    return D_801CA950;
}
