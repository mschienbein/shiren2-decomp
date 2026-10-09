#include "common.h"
typedef struct { s32 index; s32 cursor; } Iterator;
extern s32 func_800A8FC8(Iterator *it, s32 kind);
extern void *func_800A910C(Iterator *it);
extern void func_800E4DA8(void *obj);
void func_800C8194(void) { Iterator it; Iterator *p = &it; p->index = 0; while (func_800A8FC8(p, 0x7C)) func_800E4DA8(func_800A910C(p)); }
