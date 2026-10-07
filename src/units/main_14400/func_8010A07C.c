#include "common.h"

typedef unsigned char u8;

void *func_800A38FC(s32);
void *func_80109F50(void *, u8);
void *func_8010A07C(s32 a) { return func_80109F50(func_800A38FC(0xE0), a); }
