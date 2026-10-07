#include "common.h"
typedef unsigned short u16;
typedef struct { u16 id; char pad[10]; } Entry;
typedef struct { char pad[0x50]; Entry *entries; } S;
extern char *func_80048480(u16 id);
extern char *func_80083C90(char *dst, char *src);
void func_800972D0(S *p, s32 i, char *c) { func_80083C90(c, func_80048480(p->entries[i].id)); }
