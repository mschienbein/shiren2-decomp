#include "common.h"

/* Same field view as the accepted seed helper; its final condition is unused. */
typedef struct {
    unsigned char unknown_00[0x10];
    u32 field_10;
    u32 field_14;
    u32 field_18;
} State_800C5E8C_Partial;
extern u32 func_800C5E8C(State_800C5E8C_Partial *state, u32 seed);

void func_800C5DF4(State_800C5E8C_Partial *state, u32 seed) {
    func_800C5E8C(state, seed);
}
