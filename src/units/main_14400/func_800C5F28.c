#include "common.h"

typedef struct {
    char reserved_00[0xC];
    void *field_0C;
} State;
extern s32 D_80149DC8[];
extern void func_800D8FA8(State *);

void func_800C5F28(State *state, s32 flags) {
    state->field_0C = D_80149DC8;
    if (flags & 1) {
        func_800D8FA8(state);
    }
}
