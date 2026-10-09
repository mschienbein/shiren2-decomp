#include "common.h"
extern unsigned char D_801C54B9[];
extern void *func_800ACBB8(void *);
extern unsigned char *func_80083F34(void *, s32, void *);
unsigned char *func_800ACBE4(void *self) { unsigned char *result; void *data = func_800ACBB8(self); if (data) { *func_80083F34(data, 4, D_801C54B9) = 0; result = D_801C54B9; } else { result = 0; } return result; }
