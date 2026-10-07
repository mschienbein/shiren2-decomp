#include "common.h"

typedef struct { char pad[0x1C]; unsigned short unk1C; } S;
void func_800A81C4(S *s) { s->unk1C |= 0x80; }
