#include "common.h"
extern s32 func_800E0C2C(void *object);
extern s32 func_80049CB4(s32 command, ...);
/* Item vtable slot +0x44 (self, unit). `self` is the adjusted receiver the dispatcher supplies
 * and is unused here; `unit` is forwarded directly as func_800E0C2C's object. */
void func_80118830(void *self, void *unit) { if (func_800E0C2C(unit)) func_80049CB4(0x128, 0x77); }
