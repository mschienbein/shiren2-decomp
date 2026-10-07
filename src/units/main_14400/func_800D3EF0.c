#include "common.h"
extern char D_80147620[];
typedef unsigned char u8;
extern u8 func_800C57A0(void *rng);
extern void func_800D3F44(void *);
extern void func_800D4038(void *);
void func_800D3EF0(void *p) {
    if (func_800C57A0(D_80147620) & 1) func_800D3F44(p);
    else func_800D4038(p);
}
