#include "common.h"

typedef unsigned short u16;
extern unsigned short D_801569BA,D_801569BC;
extern char D_80147620[];
extern s32 func_800C5954(void *,u16,u16);
extern void func_800AE6C4(void *,unsigned short);
void func_800AB6B8(void *obj) { func_800AE6C4(obj,func_800C5954(D_80147620,D_801569BA,D_801569BC)); }
