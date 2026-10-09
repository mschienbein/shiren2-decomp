#include "common.h"
typedef struct { s32 field_0, field_4, field_8, field_C, field_10, field_14, field_18, field_1C; char messages_20[4][0x100]; } Slot;
extern Slot D_80161724[1];
void func_8005498C(s32 value) { Slot *state = &D_80161724[0]; if (state->field_0 != -1) state->field_8 = value; }
