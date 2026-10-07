#include "common.h"

typedef unsigned char u8;

extern s32 func_80041FF8(void);
extern s32 func_80041E50(void);
extern s32 func_800627C4(void);
extern s32 func_800627D4(void);

s32 func_8007E04C(void) {
    s32 result = 1;

    if ((u8)func_80041FF8()) {
        result = 0;
    } else if ((u8)func_80041E50()) {
        result = 0;
    } else if (func_800627C4() == 1 && func_800627D4() == 0x12) {
        result = 0;
    }
    return result;
}
