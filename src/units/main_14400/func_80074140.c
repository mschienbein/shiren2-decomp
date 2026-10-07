#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern s32 D_8013D8CC;
void func_80074174(s32);

s32 func_80074140(void) {
    if (D_8013D8CC == 0) {
        return -1;
    }
    func_80074174(0);
    return 0;
}
