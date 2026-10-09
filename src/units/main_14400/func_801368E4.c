#include "common.h"
extern u32 func_800B1C6C(void *position);
s32 func_801368E4(void *position) { return (func_800B1C6C(position) & 0xE100) == 0; }
