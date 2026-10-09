#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

extern void *func_800E8A68(void *obj, u8 arg1);

void *func_800EB064(void *obj) {
    return func_800E8A68(obj, 9);
}
