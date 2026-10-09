#include "common.h"

typedef struct {
    char reserved_00[2];
    unsigned char field_02;
} State;
typedef struct { s32 x, y; } Pos;
extern s32 func_80049CB4(s32, ...);

void func_80115DC8(State *state, void *player, Pos *position, unsigned char second) {
    state->field_02 &= 0xEF;
    if (player == 0) {
        func_80049CB4(0x10D7, position);
    }
    if (second != 0) {
        func_80049CB4(0x129, second);
    }
}
