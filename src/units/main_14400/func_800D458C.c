#include "common.h"

/* 0x10-byte room state (D_80143434): room pointer +0 (stored by func_800D459C),
 * words +4/+8/+0xC. */
typedef struct {
    void *room;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
} State;

void func_800D458C(State *state) {
    state->field_08 = 0;
    state->field_0C = 0;
    state->room = 0;
}
