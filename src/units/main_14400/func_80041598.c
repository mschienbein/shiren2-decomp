#include "common.h"
extern s32 *func_800B1FD4(unsigned char);
s32 func_80041598(unsigned char index, s32 *out) { s32 result = -1; s32 *src = func_800B1FD4(index); if (src) { out[0] = src[1]; out[1] = src[0]; out[2] = src[3]; out[3] = src[2]; result = 0; } return result; }
