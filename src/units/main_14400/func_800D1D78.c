#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0xBB]; u8 bBB[12]; } S;
extern const u8 D_80154718[20];
void func_800D1D78(S *s, s32 i) { s->bBB[D_80154718[i]] = i; }
