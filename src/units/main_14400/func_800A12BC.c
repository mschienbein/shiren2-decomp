#include "common.h"
typedef struct { char pad[0x10]; void *unk10; } S;
s32 func_800A1308(S *, unsigned short);
s32 func_800D8FF0(void *obj);
s32 func_800A12BC(S *s, unsigned short a) { s32 r = func_800A1308(s, a); if ((u32)(r - 6) < 2) { func_800D8FF0(s->unk10); return 8; } return r; }
