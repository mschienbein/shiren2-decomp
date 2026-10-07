#include "common.h"
typedef struct { unsigned char pad[0x9A]; unsigned short unk9A; } S;
void func_800F3AE0(S *s) { s->unk9A &= ~0x20; }
