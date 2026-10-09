#include "common.h"

extern const unsigned char D_80156CB8[24];
extern unsigned char func_800A9958(void);
s32 func_800AA8D0(void) { return D_80156CB8[func_800A9958()] != 0; }
