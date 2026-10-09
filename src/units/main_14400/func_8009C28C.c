#include "common.h"

extern const unsigned char D_80151E38[144];
extern void func_800D8FA8(void *object);

void func_8009C28C(void **object, s32 flags) {
    ((const void **)object)[0x4C / 4] = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(object);
    }
}
