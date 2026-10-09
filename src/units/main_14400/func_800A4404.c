#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair;
typedef struct {
    char reserved_00[9];
    unsigned char field_09;
} State;
extern u32 func_800B1C6C(Pair *);
extern s32 func_800A58B8(State *);
extern s32 func_800B5900(Pair *, s32, s32, s32 *);
extern s32 func_800A422C(State *, Pair *, s32);

static inline Pair *copy_pair(Pair *destination, Pair *source) {
    destination->x = source->x;
    destination->y = source->y;
    return destination;
}

s32 func_800A4404(State *state, Pair *position, s32 *output) {
    Pair copy;
    Pair *point;
    s32 direction;
    s32 result;
    *output = 0;
    if (func_800B1C6C(position) & 0x8000) {
        return 0;
    }
    point = copy_pair(&copy, position);
    direction = state->field_09 & 0xF;
    result = func_800B5900(point, direction, func_800A58B8(state), output);
    if (result == 0 && *output == 0) {
        if ((func_800B1C6C(position) & 0x4000) &&
            func_800A422C(state, position, state->field_09 & 0xF)) {
            return 1;
        }
    }
    return result;
}
