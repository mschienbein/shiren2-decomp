#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

u32 func_8009FAF8(void *record);
s32 func_8009FCB4(void *obj);
void func_80045A24(s32 flag);
s32 func_8009FA94(void *obj, s32 msg) {
    u32 failed;

    if (msg == 0x36) {
        failed = func_8009FAF8(obj) ^ 1;
        if (failed) {
            return -1;
        }
        if (func_8009FCB4(obj)) {
            func_80045A24(0);
        } else {
            func_80045A24(1);
        }
    }
    return -1;
}
