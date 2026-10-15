#include "common.h"
#include "map_rect.h"

typedef struct {
    char reserved_00[9];
    unsigned char field_09;
} State;
extern u32 func_800B1C6C(MapPos *);
extern s32 func_800A58B8(State *);
extern s32 func_800B5900(void *, s32, s32, void **);
extern s32 func_800A422C(State *, MapPos *, s32);

static inline MapPos *copy_pair(MapPos *destination, MapPos *source) {
    destination->x = source->x;
    destination->y = source->y;
    return destination;
}

s32 func_800A4404(State *state, MapPos *position, void **output) {
    MapPos copy;
    MapPos *point;
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
