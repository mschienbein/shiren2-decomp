#include "common.h"

typedef struct { u32 w0; u32 w1; } Gfx;

/* Optional display-list callback: func_80055078 calls it with the current
 * graphics cursor and continues with the returned cursor (installed target:
 * func_80064444).
 */
typedef Gfx *(*GfxCallback80055068)(Gfx *gdl);

extern GfxCallback80055068 D_801630B8;

void func_80055068(GfxCallback80055068 callback)
{
    D_801630B8 = callback;
}
