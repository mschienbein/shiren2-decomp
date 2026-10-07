#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

s32 D_80138BD0 = 1;
void func_80051E78(void);
void func_80045A60(void) {
    D_80138BD0 = 1;
    func_80051E78();
}
