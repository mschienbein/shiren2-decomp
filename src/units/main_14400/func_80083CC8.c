#include "common.h"
char *func_80083CC8(char *a, char *b, u32 n) { char *p = a; while (n--) { if (!(*p++ = *b++)) break; } return a; }
