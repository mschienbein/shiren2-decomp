#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

char *func_80048480(u16 id);
char *func_800C4EE0(u16 *id) {
    return func_80048480(*id);
}
