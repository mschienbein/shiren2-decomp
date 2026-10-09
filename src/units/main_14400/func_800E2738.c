#include "common.h"

/* Slot +0x94 targets func_800E115C / derived dispatchers; +0x90 is zero in their tables. */
typedef struct {
    char reserved_00[0x90];
    short adjustment_90;
    short reserved_92;
    s32 (*method_94)(void *, s32, s32, unsigned char, s32);
} Methods;
typedef struct {
    char reserved_00[0x24];
    Methods *methods_24;
} State;

void func_800E2738(State *state) {
    Methods *methods = state->methods_24;
    methods->method_94((char *)state + methods->adjustment_90, 1, 0x13, 0, 0);
}
