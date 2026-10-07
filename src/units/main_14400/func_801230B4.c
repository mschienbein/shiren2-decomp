#include "common.h"
extern char *func_800AC990(void *obj);
extern char *func_80083C90(char *dst, char *src);
char *func_801230B4(void *arg, char *out) { func_80083C90(out, func_800AC990(arg)); return out; }
