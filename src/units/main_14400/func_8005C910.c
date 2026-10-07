#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 total; s16 remaining; u8 pad4; u8 mode; } State8005C910;
extern State8005C910 D_80165952;
void func_8005C890(s32 mode, s32 arg1, s32 arg2);
void func_8005C910(s32 mode, s32 arg1, s32 arg2) {
    State8005C910 *state = &D_80165952;
    if (state->total == state->remaining && state->total != 0) {
        return;
    }
    if (mode != 0 && state->total != 0) {
        if (state->mode == 0) {
            state->remaining = state->total - state->remaining;
        }
        state->mode = mode;
    } else {
        func_8005C890(mode, arg1, arg2);
    }
}
