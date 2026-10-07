#include "common.h"

typedef struct { u32 w0; u32 w1; } Gfx;

/* Reset of five 32-bit BSS words. D_801630B0/B4 are numeric mode/index words.
 * D_801630B8 is the optional display-list callback installed by func_80055068
 * and called by func_80055078 with the graphics cursor. D_801630BC holds the
 * 0x25800-byte func_8006A8D8 buffer and D_801630C0 points 0x12C00 bytes into
 * it; both are forwarded to the drawing routine as buffer pointers.
 */
extern u32 D_801630B0;
extern u32 D_801630B4;
extern Gfx *(*D_801630B8)(Gfx *gdl);
extern void *D_801630BC;
extern void *D_801630C0;

/* The observed caller supplies no meaningful arguments and ignores a return.
 * Original source function name and complete prototype remain unknown.
 */
void func_80054DB0(void)
{
    D_801630B0 = 0;
    D_801630B4 = 0;
    D_801630B8 = 0;
    D_801630BC = 0;
    D_801630C0 = 0;
}
