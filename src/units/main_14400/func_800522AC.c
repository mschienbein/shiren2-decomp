#include "common.h"
typedef struct { signed char a; signed char b; } Pair;
extern Pair D_8014BF98;
void func_80051E9C(short, Pair);
void func_800522AC(short a0, unsigned char a1){ Pair p = D_8014BF98; p.a = a1; func_80051E9C(a0, p); }
