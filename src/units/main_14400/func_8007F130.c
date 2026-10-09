#include "common.h"

#include "row_views.h"
typedef struct { u32 w0, w1; } Gfx;
typedef struct { Gfx commands[4096]; u8 *pixels; } RenderBuffer;
extern u8 *D_801A9050[2];
extern RenderBuffer D_801AA168, D_801B2370;
extern const char D_8014D268[], D_8014D278[];
/* The original allocator ignores its diagnostic-name argument. */
extern void *func_8006A8D8(const char *name, u32 size);

void func_8007F130(void) {
    s32 i, shade;
    D_801A9050[0] = func_8006A8D8(D_8014D268, 0x9600);
    D_801A9050[1] = func_8006A8D8(D_8014D278, 0x9600);
    for (i = 9; i >= 0; i--) D_801A9080[i].field_02 = 0;
    D_801AA168.pixels = D_801A9050[0];
    D_801B2370.pixels = D_801A9050[1];
    for (i = 0; i < 0x9600;) {
        shade = (((i >> 2) & 7) + i / 1280) % 8 + 1;
        shade |= shade << 4;
        D_801A9050[0][i] = shade;
        D_801A9050[1][i] = shade;
        i++;
        D_801A9050[0][i] = shade;
        D_801A9050[1][i] = shade;
        i++;
        D_801A9050[0][i] = shade;
        D_801A9050[1][i] = shade;
        i++;
        shade &= ~15;
        D_801A9050[0][i] = shade;
        D_801A9050[1][i] = shade;
        i++;
    }
    D_801AA168.commands[0].w0 = 0xDF000000;
    D_801AA168.commands[0].w1 = 0;
    D_801B2370.commands[0].w0 = 0xDF000000;
    D_801B2370.commands[0].w1 = 0;
}
