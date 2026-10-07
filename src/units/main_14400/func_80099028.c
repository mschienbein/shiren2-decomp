#include "common.h"

typedef struct { char pad[0x20]; s32 x20; } S;
s32 func_80098F54(S *, s32);
s32 func_80099028(S *s, s32 id) { s32 v = func_80098F54(s, id); if (v < 0) return v; return v / s->x20; }
