#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[0x55]; u8 f55; } S;
void func_800E4D90(S *a, s32 b){a->f55 = (a->f55 & 0xF) | (b<<4);}
