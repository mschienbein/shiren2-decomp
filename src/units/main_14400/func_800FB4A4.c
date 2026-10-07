#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0x24]; void *vtable; } S;
extern u8 D_8015A130[];
void func_800EFD28(S *, s32);
void func_800A3918(void *);
void func_800FB4A4(S *s, s32 flags) { s->vtable = D_8015A130; func_800EFD28(s, 0); if (flags & 1) func_800A3918(s); }
