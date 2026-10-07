#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0xCE]; u8 bCE; u8 bCF; u8 pad2[8]; u8 bD8; } S;
u8 *func_801297E8(S *s, u8 *p) { s->bD8 = *p++; s->bCE = *p++; s->bCF = *p++; return p; }
