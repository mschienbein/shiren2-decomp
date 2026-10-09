#include "common.h"

typedef struct {
    unsigned char field_00;
    char reserved_01[3];
    unsigned char field_04;
} State;
extern s32 func_8008C89C(State *);

s32 func_8008C908(State *state) {
    s32 result = 0;
    if (state->field_00 == 1) {
        if (state->field_04 != 0) {
            state->field_04--;
        }
        result = func_8008C89C(state);
    }
    return result;
}
