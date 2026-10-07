#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
extern unsigned char D_80142F09;
extern void *func_800ABFE4(s32 index);
extern void *func_800AB16C(void *object, u8 a, s32 b);
void *func_800AAEB4(s8 index) {
    if (index == -1) index = D_80142F09;
    index--;
    return func_800AB16C(func_800ABFE4((unsigned char)index), 0, 0);
}
