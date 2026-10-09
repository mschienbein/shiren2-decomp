#include "common.h"

typedef unsigned short u16;
extern s32 func_80041E7C(void);
extern void func_800740A8(s32 value);
extern u16 func_80058D60(void);
/* D_8013D3F0 supplies both input records; this handler ignores the first. */
s32 func_8006E2E8(void *self, u16 *input)
{
    s32 result = -1;
    if (func_80041E7C() == 0 && (*input & 0x4000)) {
        func_800740A8(1);
        return result;
    }
    if (*input & 0xF) {
        result = 8;
    } else if ((*input | func_80058D60()) & 0x9000) {
        result = 2;
    }
    return result;
}
