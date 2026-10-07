#include "common.h"

typedef unsigned char u8;

void *func_800A38FC(s32);
void *func_801091D0(void *, u8);
void *func_801092C0(u8 arg) { return func_801091D0(func_800A38FC(0xC4), arg); }
