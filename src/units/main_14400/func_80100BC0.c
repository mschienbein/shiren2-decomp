#include "common.h"

/* +0x58 is the optional target object (null-capable), not an integer. */
typedef struct { unsigned char fields_00[0x58]; void *target_58; unsigned char fields_5C[0x2D]; unsigned char field_89; } State;
extern unsigned char func_800A6420(State *, void *target);
extern s32 func_800A65B8(State *, void *target);
extern s32 func_800A692C(State *, s32);
extern s32 func_800E1CD4(State *, s32);
extern s32 func_800E7104(State *);
extern s32 func_800E8350(State *);
extern void func_800F06E4(State *);
extern s32 func_800F1024(State *);
extern s32 func_800F3310(State *);
s32 func_80100BC0(State *state) {
    s32 kind;
    if (func_800A692C(state, 0x12)) return func_800F3310(state);
    kind = func_800A6420(state, state->target_58) & 0xFF;
    if (kind < 3 && kind != 0) {
        s32 amount = func_800A65B8(state, state->target_58);
        s32 enough = (state->field_89 < amount) ^ 1;
        if (enough && func_800F1024(state)) { func_800F06E4(state); return 0; }
    }
    {
        s32 result;
        if (!func_800E1CD4(state, 0x10)) result = func_800E7104(state);
        else result = func_800E8350(state);
        return result;
    }
}
