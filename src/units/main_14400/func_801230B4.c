#include "common.h"
#include "item_records.h"
extern char *func_800AC990(void *obj);
extern char *func_80083C90(char *dst, char *src);
char *func_801230B4(Item *arg, char *out) { func_80083C90(out, func_800AC990(arg)); return out; }
