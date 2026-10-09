#include "common.h"
typedef unsigned char u8;
typedef struct {
    void *pool_00;
    const void *vtable_04;
    u8 state_08[8];
    u8 entries_10[8];
    void *owner_18;
} State;
extern const u8 D_80154550[];
extern void *func_800CE620(void *, u8 *, u8);
State *func_800CFF00(State *state, void *owner, u8 capacity) {
    func_800CE620(state, state->entries_10, capacity);
    state->vtable_04 = D_80154550;
    state->owner_18 = owner;
    return state;
}
