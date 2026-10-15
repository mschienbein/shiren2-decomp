#include "common.h"

#include "row_views.h"
typedef struct { u32 w0, w1; } Gfx;
/* func_8007F350 selects two 0x8208-byte arenas at D_801A9F68.
   The former D_801AA168 and D_801B2370 labels are their +0x200 command fields. */
typedef struct {
    u16 palette[0x100];
    Gfx commands[0x1000];
    u8 *pixels;
    u8 pad8204[4];
} RenderBuffer;
extern u8 *D_801A9050[2];
extern RenderBuffer D_801A9F68[2];
extern const char D_8014D268[], D_8014D278[];
/* The original allocator ignores its diagnostic-name argument. */
extern void *func_8006A8D8(const char *name, u32 size);

/* ODD_C: access the command field through its containing arena.  The
   explicit field address also preserves the ROM's paired word stores. */
static inline Gfx *arena_commands(RenderBuffer *arena) {
    return arena->commands;
}

void func_8007F130(void) {
    s32 i, shade;
    D_801A9050[0] = func_8006A8D8(D_8014D268, 0x9600);
    D_801A9050[1] = func_8006A8D8(D_8014D278, 0x9600);
    for (i = 9; i >= 0; i--) D_801A9080[i].field_02 = 0;
    D_801A9F68[0].pixels = D_801A9050[0];
    D_801A9F68[1].pixels = D_801A9050[1];
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
    {
        Gfx *packet = arena_commands(&D_801A9F68[0]);
        packet->w0 = 0xDF000000;
        packet->w1 = 0;
    }
    {
        Gfx *packet = arena_commands(&D_801A9F68[1]);
        packet->w0 = 0xDF000000;
        packet->w1 = 0;
    }
}
