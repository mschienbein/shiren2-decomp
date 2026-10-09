#include "common.h"

extern unsigned short *func_80044F5C(unsigned char, unsigned char);
unsigned short func_800EFDAC(unsigned char index, unsigned char kind) { unsigned short *entry = func_80044F5C(index, kind); if (!entry) { entry = func_80044F5C(index, 1); if (!entry) return 0; } return *entry; }
