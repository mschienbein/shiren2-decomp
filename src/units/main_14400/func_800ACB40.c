#include "common.h"
extern char D_801C54B0[];
extern char *func_800ACAEC(void *);
extern char *func_80083F34(void *, s32, char *);
char *func_800ACB40(void *item) { char *p = func_800ACAEC(item); char *result; if (p) { char *out = D_801C54B0; *func_80083F34(p, 4, out) = 0; result = out; } else { result = 0; } return result; }
