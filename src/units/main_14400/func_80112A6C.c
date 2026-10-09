#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern const u16 D_80157A7C[][3];
char *func_80048480(u16 id);

char *func_80112A6C(u8 kind, s32 level) {
    return func_80048480(D_80157A7C[kind - 0xE9][level - 1]);
}
