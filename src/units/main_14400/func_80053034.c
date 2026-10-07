#include "common.h"
typedef struct { short key; char pad[0xA]; } Entry12;
extern Entry12 D_801397E0[];
Entry12 *func_80053034(short key){ s32 i; Entry12 *e = D_801397E0; for (i = 14; i != -1; i--, e++) { if (e->key == key) return e; } return 0; }
