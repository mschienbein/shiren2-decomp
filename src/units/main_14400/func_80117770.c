#include "common.h"

typedef struct {
    char reserved_00[0x78];
    short adjustment_78;
    short reserved_7A;
    void (*method_7C)(void *, short);
} Methods;
typedef struct {
    char reserved_00[0x24];
    Methods *methods_24;
} State;
extern unsigned short D_801569D2;
extern s32 func_800A99D0(void);
extern void func_800498E4(s32, ...);
extern s32 func_800E1CC4(State *, s32);

/* context is unused but supplied as the original virtual action receiver. */
void func_80117770(void *context, State *state) {
    if (func_800A99D0()) {
        func_800498E4(0x222);
    } else {
        s32 multiplier;
        Methods *methods;
        if (func_800E1CC4(state, 3)) {
            multiplier = 2;
        } else {
            multiplier = 1;
        }
        methods = state->methods_24;
        methods->method_7C((char *)state + methods->adjustment_78, (short)(D_801569D2 * multiplier));
    }
}
