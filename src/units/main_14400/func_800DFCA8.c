#include "common.h"

typedef struct {
    unsigned short field_00;
    unsigned short field_02;
    void *field_04;
} State;
extern s32 D_80157FA8[];
extern s32 D_80158B38[];

/* The parser factory supplies its payload pointer; this action has no payload. */
State *func_800DFCA8(State *state, unsigned char *unused_data) {
    state->field_04 = D_80157FA8;
    state->field_00 = 0x30;
    state->field_04 = D_80158B38;
    return state;
}
