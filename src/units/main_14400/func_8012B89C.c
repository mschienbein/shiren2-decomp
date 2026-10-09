#include "common.h"

typedef struct {
    unsigned char pad00[0x40];
    s32 field40;
    unsigned char pad44[0x50];
    s32 field94;
    unsigned char pad98[0x25];
    unsigned char flagsBD;
    unsigned char padBE[0x1B];
    unsigned char fieldD9;
    unsigned char fieldDA;
} State;

void func_8012B89C(State *state)
{
    state->fieldD9 = 0;
    state->field94 = state->field40;
    state->fieldDA = state->flagsBD & 0x40;
}
