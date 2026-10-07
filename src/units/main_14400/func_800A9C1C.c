#include "common.h"
extern s32 func_800A9BC0(unsigned char id, unsigned char *matched);
static inline s32 finish(s32 found, unsigned char *result) { if (!found) return 0; *result = 5; return 1; }
s32 func_800A9C1C(unsigned char value, unsigned char *result) { return finish(func_800A9BC0(value, result), result); }
