#include "common.h"
typedef struct { unsigned char pad[0x1E]; unsigned char unk1E; } S;
void func_800E20F0(S *);
/* The caller supplies item; this forwarding implementation does not inspect it. */
void func_80111DD8(void *item, S *s) { if (s->unk1E & 0x7C) func_800E20F0(s); }
