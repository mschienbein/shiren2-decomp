#include "common.h"

typedef struct { char pad[4]; short f4; } S;
void func_80059728(void*);
void func_8005ABBC(void*, s32, s32);
void func_80086A28(S *a){ char buf[0x20]; func_80059728(buf); func_8005ABBC(buf, 8, 1); a->f4 = 4;}
