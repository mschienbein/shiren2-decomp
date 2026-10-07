#include "common.h"
extern s32 func_800AFBFC(void *);
extern void *func_800AFB80(void *);
extern void func_800AFC6C(void *, void *);
void func_800AC68C(void *a) { if (!func_800AFBFC(a)) { void *p = func_800AFB80(a); if (p) func_800AFC6C(p, a); } }
