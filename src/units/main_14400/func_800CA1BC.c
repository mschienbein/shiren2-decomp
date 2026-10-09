#include "common.h"

typedef struct {
    char reserved_00[0x18];
    short adjustment_18;
    short reserved_1A;
    void (*method_1C)(void *, s32, s32 *);
} Methods;
typedef struct {
    char reserved_00[8];
    s32 field_08;
    char reserved_0C[0xC];
    Methods *methods_18;
} State;
extern s32 func_800CA118(State *);
extern void func_800CA0A8(State *, s32);

void func_800CA1BC(State *state) {
    Methods *methods;
    state->field_08 = func_800CA118(state);
    func_800CA0A8(state, 4);
    methods = state->methods_18;
    methods->method_1C((char *)state + methods->adjustment_18, 4, &state->field_08);
}
