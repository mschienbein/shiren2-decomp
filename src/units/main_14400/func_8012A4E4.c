#include "common.h"
typedef struct { char pad[0x44]; s32 unk44; char pad48[0x13C - 0x48]; } E;
extern s32 D_801CA6D4;
extern E *D_801CA6DC;
s32 func_8012A4E4(s32 id) { s32 i, n; E *e; if (id == 0) return 0; n = 0; e = D_801CA6DC; for (i = 0; i < D_801CA6D4; i++, e++) { if (e->unk44 == id) n++; } return n; }
