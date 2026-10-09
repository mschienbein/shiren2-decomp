#include "common.h"

extern unsigned char D_80147600[];
extern void *func_800C5C50(void *object);
/* The constructor's returned receiver is intentionally discarded. */
void func_800C5D1C(void) { func_800C5C50(D_80147600); }
