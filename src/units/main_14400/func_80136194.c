#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

void func_800F479C(void *obj, s32 flags);
void func_800A3918(void *obj);
void func_80136194(void *obj, s32 flags) {
    func_800F479C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
