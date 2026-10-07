#include "common.h"

typedef unsigned char u8;

void func_800486A4(void *, void *, void *); void func_80048650(void *); void func_80048794(void *);
void func_800950A8(u8 *s, void *arg) { u8 *p = s + 4; func_800486A4(p, arg, s); func_80048650(p); func_80048794(p); }
