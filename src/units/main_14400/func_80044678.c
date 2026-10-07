#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

void *func_80043690(s32 size);
void *func_800439A8(void *obj, s32 kind, s32 a, s32 *status);
void *func_800442E0(s32 size);
void *func_80044308(void *obj, s32 a, s32 *status);

void *func_80044678(s32 kind, s32 a, s32 *status) {
    void *obj;
    if (kind != 2) {
        obj = func_800439A8(func_80043690(0x50), kind, a, status);
    } else {
        obj = func_80044308(func_800442E0(0x24), a, status);
    }
    return obj;
}
