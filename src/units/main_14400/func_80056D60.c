#include "common.h"

/* The catalogue-adjacent setters share this initialized flag. */
s32 D_8013A280 = 0;

void func_80056D60(void) {
    D_8013A280 = 1;
}

void func_80056D70(void) {
    D_8013A280 = 0;
}
