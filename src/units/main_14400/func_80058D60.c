#include "common.h"
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct { u16 field_00; u8 x_02, y_03; u16 field_04, pressed_06, repeat_08, accumulated_0A, previous_0C; u8 delay_0E; } Input;
extern Input D_80163124;
u16 func_80058D60(void) { return D_80163124.accumulated_0A; }
