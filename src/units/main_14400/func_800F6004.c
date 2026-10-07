#include "common.h"

extern void *func_800A38FC(s32);
extern void *func_800F5F60(void *, unsigned char);
void *func_800F6004(unsigned char a) { return func_800F5F60(func_800A38FC(0xA8), a); }
