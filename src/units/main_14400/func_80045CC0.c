#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_8005275C(void);
/* Returns the track id narrowed to short in the body (sll/sra at 0x80045CD0);
 * the int result is used un-narrowed by func_80045754 (0x80045770). */
s32 func_80045CC0(void) {
    return (s16)func_8005275C();
}
