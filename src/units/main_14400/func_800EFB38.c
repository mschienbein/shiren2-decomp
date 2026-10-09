#include "common.h"

typedef struct { unsigned char reserved_00[0xA]; unsigned char field_0A; } State;
extern void *func_800D4EA0(u32 index);
void *func_800EFB38(State *state) {
    unsigned char value = state->field_0A;
    s32 in_range = 0;
    if (value >= 0x96) in_range = value < 0xA6;
    if (in_range) return func_800D4EA0(1);
    return func_800D4EA0(0);
}
