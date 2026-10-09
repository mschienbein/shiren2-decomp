#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u16 field_00; u8 x_02, y_03; u16 field_04, pressed_06, repeat_08, accumulated_0A, previous_0C; u8 delay_0E; } Input;
extern Input D_80163124;
void func_80058D50(void) { D_80163124.accumulated_0A = 0; }
