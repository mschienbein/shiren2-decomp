#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

char *func_80114794(u8 *obj);
char *func_800ACBB8(u8 *obj) {
    if (*obj == 9) {
        return func_80114794(obj);
    }
    return 0;
}
