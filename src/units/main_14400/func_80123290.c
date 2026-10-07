#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_80114E28(void *obj, s32 *msg);
void func_80114404(void *obj, s32 a, s32 b, s32 c, s32 d);
s32 func_80123290(void *obj, s32 *msg) {
    if (*msg == 0x1E) {
        func_80114404(obj, 1, 1, 0xA, 0x14);
        return 1;
    }
    return func_80114E28(obj, msg);
}
