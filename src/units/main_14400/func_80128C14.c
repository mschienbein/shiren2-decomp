#include "common.h"

typedef struct { char pad[0xC]; unsigned char unkC; } S;
void func_80128C14(S *s, s32 v) { s->unkC |= v; }
