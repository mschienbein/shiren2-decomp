#include "common.h"

typedef struct {
    char reserved_00[0xBC];
    s32 field_BC;
} State;

void func_800FB254(State *state) {
    state->field_BC = 1;
}
