#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

char *func_800AC990(void *obj);
void func_800498E4(s32 message_id, ...);
s32 func_800CFE40(s32 a0, s32 a1, u8 *p, s32 flag) {
    s32 r = *p != 10;
    if (flag && !r) {
        func_800498E4(0x85, func_800AC990(p));
    }
    return r;
}
