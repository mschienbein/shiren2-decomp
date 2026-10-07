#include "common.h"

typedef unsigned char u8;
extern void *func_800AC244(u8);
extern void func_800AB35C(void *,s32);
void *func_800AADB4(unsigned char id,s32 value) { void *obj=func_800AC244(id); func_800AB35C(obj,value); return obj; }
