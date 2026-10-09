#include "common.h"

typedef struct {
    char reserved_00[0x35];
    unsigned char fields_35[10];
} State;

s32 func_800E1DD8(State *state) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (state->fields_35[i] != 0) {
            return 1;
        }
    }
    return 0;
}
