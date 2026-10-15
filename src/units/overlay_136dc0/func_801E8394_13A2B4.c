#include "func_801E8394_13A2B4/types.h"
extern s32 D_801DE9AC, D_801E4E78, D_801DE9B0, D_801DEAAC;
void func_801E8394_13A2B4(void) {
    S06Transition *state = &D_801F71F0_149110;
    s32 index;
    if (!state->duration) return;
    for (index = 3; index != -1; index--) {
        float value;
        if (state->targets[index] == -1) continue;
        value = (float)state->targets[index] - (float)state->starts[index];
        value = value * state->frame / state->duration;
        value += state->starts[index];
        switch (index) {
        case 0: D_801DE9AC = (u32)value; break;
        case 1: D_801E4E78 = (u32)value; break;
        case 2: D_801DE9B0 = (u32)value; break;
        case 3: D_801DEAAC = (u32)value; break;
        }
    }
    state->frame++;
    if (state->duration < state->frame) state->duration = 0;
}
