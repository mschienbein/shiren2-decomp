#include "common.h"
/* Two allocated image buffers, initialized at 8007F148 and 8007F160. */
extern unsigned char *D_801A9050[2];
void *func_80083908(s32 index) { return D_801A9050[index]; }
