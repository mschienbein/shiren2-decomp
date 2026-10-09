#include "common.h"

typedef struct {
    char reserved_00[8];
    void *field_08;
} State;
extern s32 D_80153AA0[];
extern void func_800AC68C(State *);

void func_801271E8(State *state, s32 flags) {
    state->field_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(state);
    }
}
