#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Counts newline characters, skipping the second byte of 0xF0-prefixed codes. */
s32 func_80053C80(char *str) {
    s32 lines = 0;

    while (*str != 0) {
        if ((*str & 0xF0) == 0xF0) {
            str++;
        } else if (*str == '\n') {
            lines++;
        }
        str++;
    }
    return lines;
}
