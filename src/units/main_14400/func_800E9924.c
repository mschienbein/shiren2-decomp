#include "common.h"

typedef struct {
    char reserved_00[0x6C];
    s32 field_6C;
    s32 reserved_70;
    unsigned char field_74;
} State;
extern unsigned short func_800E08B0(State *);
extern void func_800E9990(State *, s32, s32);

void func_800E9924(State *state, s32 amount) {
    if (amount != 0 && func_800E08B0(state) != 0) {
        if (state->field_74 != 0) {
            state->field_6C += amount;
        } else {
            func_800E9990(state, amount, 1);
        }
    }
}
