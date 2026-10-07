#include "common.h"
typedef struct { u32 w0; u32 w1; } Gfx;
extern char D_8013B810[];
Gfx *func_80062E88(Gfx *g){ g->w0 = 0xDE000000; g->w1 = (u32)D_8013B810; return g + 1; }
