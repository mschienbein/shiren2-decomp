#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

void *func_80044D1C(void *obj, u8 value);
void *func_8010A7AC(void *obj, u8 value) {
    return func_80044D1C(obj, value);
}
