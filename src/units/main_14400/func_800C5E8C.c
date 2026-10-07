#include "common.h"

/* Observed word fields only: offsets 16/20/24. The original complete type,
 * allocation, ownership and TU remain unknown; no sizeof is used. */
typedef struct {
    unsigned char unknown_00[0x10];
    u32 field_10;
    u32 field_14;
    u32 field_18;
} State_800C5E8C_Partial;

/* Return the measured 0/1 final unsigned condition seen in v0 and propagated
 * by both direct wrappers. This is not a historical return declaration claim.
 * Original seed signedness and API typedefs remain unresolved. */
u32 func_800C5E8C(State_800C5E8C_Partial *state, u32 seed)
{
    u32 value;

    value = seed ^ 0xE14F56C6U;
    state->field_10 = value;
    if (value < 2U) {
        state->field_10 = 0xE14F56C6U;
    }

    value = seed ^ 0x35E73091U;
    state->field_14 = value;
    if (value < 8U) {
        state->field_14 = 0x35E73091U;
    }

    value = seed ^ 0x4AEDDA63U;
    state->field_18 = value;
    if (value < 16U) {
        state->field_18 = 0x4AEDDA63U;
    }
    return value < 16U;
}
