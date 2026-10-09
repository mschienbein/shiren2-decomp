#include "common.h"

/* Collection/item source link; func_800DA8A0 copies it to +8. */
typedef struct { void *collection; void *item; } Link;
typedef struct { s32 field_00; const void *field_04; } State;
extern const unsigned char D_80158868[48];
extern void *func_800DA8A0(State *, s32, Link *);
State *func_800DD6E0(State *state, Link *source) { func_800DA8A0(state, 0x25, source); state->field_04 = D_80158868; return state; }
