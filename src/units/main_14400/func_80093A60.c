#include "common.h"
typedef signed char s8;
/* +4 state, +6 menu mode, +7 tick delay. */
typedef struct { unsigned char pad_0[4]; s8 state_4, pad_5, mode_6, delay_7; } State;
typedef struct Window Window;
typedef struct Event Event;
extern s32 D_80147670;
extern s32 func_8006D550(s32 mode);
extern void *func_80096748(void);
extern s32 func_800957C0(Window *window, void *event, s32 mode, void *arg, s32 flag);
extern Event *func_800D8FB0(u32 size);
extern Event *func_800DF770(Event *event, s32 delay);
extern Event *func_80093E8C(State *state);

static inline s32 state_value(State *state) { return state->state_4; }

Event *func_80093A60(State *state) {
    unsigned char event[32]; /* event record filled by the modal menu */
    s32 active = 1;
    s32 inactive = state_value(state) != active;
    if (!inactive) {
        if (state->delay_7 <= 0) {
            s32 delay;
            switch (func_8006D550(5)) {
            /* ODD_C: the no-key case returns directly instead of `goto parse`;
               GCC cross-jumps it into the shared tail, and the case label it
               leaves keeps reload_cse from reusing `active` for the menu mode. */
            case -1:
                return func_80093E8C(state);
            case 8:
                if (state->mode_6) func_800957C0(func_80096748(), event, 1, 0, 0);
                goto parse;
            }
            if (state->mode_6 != active) goto parse;
            delay = D_80147670;
            delay ^= 3;
            if (delay) delay = 2;
            else delay = 4;
            state->delay_7 = delay;
        }
        return func_800DF770(func_800D8FB0(12), state->delay_7);
    parse:
        return func_80093E8C(state);
    }
    state->delay_7 = -1;
    return 0;
}
