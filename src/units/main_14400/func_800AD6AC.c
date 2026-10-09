#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
u32 func_800B1C6C(Pos *pos);
s32 func_800AD6AC(unsigned char *obj, Pos *pos) { u32 flags = func_800B1C6C(pos); s32 kind = *obj; if (kind == 0x10 && (flags & 0x2000)) return 0; if (kind == 0xF && (flags & 0x2080)) return 0; return !(flags & 0xC110); }
