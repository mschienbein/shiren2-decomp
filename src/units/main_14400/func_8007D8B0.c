#include "common.h"

typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;

#define NULL 0

/* One RDP/RSP display-list command (same view as func_8007D720.c). */
typedef struct {
    u32 command;
    u32 parameter;
} DisplayCommand;

/* libultra Gfx: one command, aligned like its long long member. */
typedef union {
    DisplayCommand words;
    long long force_structure_alignment;
} Gfx;

/* One 4x4-pixel 4bpp minimap cell: 4 rows of 2 bytes.
 * Composed as two words, blitted as bytes. */
typedef union {
    u32 w[2];
    u8 b[8];
} Pat8;

/* libultra gbi field packer. */
#define SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))

#define GFX_PUT(pkt, hi, lo)                     \
    {                                            \
        DisplayCommand *_g = (pkt);              \
        _g->command = (hi);                      \
        _g->parameter = (lo);                    \
    }

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* libultra gSPTextureRectangle: three packets claimed from pkt. */
#define TEXRECT(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)                                   \
    {                                                                                          \
        GFX_PUT(pkt, SHIFTL(0xE4, 24, 8) | SHIFTL(xh, 12, 12) | SHIFTL(yh, 0, 12),             \
                SHIFTL(tile, 24, 3) | SHIFTL(xl, 12, 12) | SHIFTL(yl, 0, 12));                 \
        GFX_PUT(pkt, SHIFTL(0xE1, 24, 8), SHIFTL(s, 16, 16) | SHIFTL(t, 0, 16));               \
        GFX_PUT(pkt, SHIFTL(0xF1, 24, 8), SHIFTL(dsdx, 16, 16) | SHIFTL(dtdy, 0, 16));         \
    }

/* libultra gSPScisTextureRectangle (clips negative screen coordinates). */
#define SCIS_TEXRECT(pkt, xl, yl, xh, yh, tile, s, t, dsdx, dtdy)                              \
    {                                                                                          \
        GFX_PUT(pkt,                                                                           \
                SHIFTL(0xE4, 24, 8) | SHIFTL(MAX((s16)(xh), 0), 12, 12) |                      \
                    SHIFTL(MAX((s16)(yh), 0), 0, 12),                                          \
                SHIFTL(tile, 24, 3) | SHIFTL(MAX((s16)(xl), 0), 12, 12) |                      \
                    SHIFTL(MAX((s16)(yl), 0), 0, 12));                                         \
        GFX_PUT(pkt, SHIFTL(0xE1, 24, 8),                                                      \
                SHIFTL((s) - (((s16)(xl) < 0)                                                  \
                                  ? (((s16)(dsdx) < 0)                                         \
                                         ? MAX((((s16)(xl) * (s16)(dsdx)) >> 7), 0)            \
                                         : MIN((((s16)(xl) * (s16)(dsdx)) >> 7), 0))           \
                                  : 0),                                                        \
                       16, 16) |                                                               \
                    SHIFTL((t) - (((yl) < 0)                                                   \
                                      ? (((s16)(dtdy) < 0)                                     \
                                             ? MAX((((s16)(yl) * (s16)(dtdy)) >> 7), 0)        \
                                             : MIN((((s16)(yl) * (s16)(dtdy)) >> 7), 0))       \
                                      : 0),                                                    \
                           0, 16));                                                            \
        GFX_PUT(pkt, SHIFTL(0xF1, 24, 8), SHIFTL(dsdx, 16, 16) | SHIFTL(dtdy, 0, 16));         \
    }

#define SET_SCISSOR(pkt, mode, ulx, uly, lrx, lry)                                             \
    GFX_PUT(pkt,                                                                               \
            SHIFTL(0xED, 24, 8) | SHIFTL((s32)((float)(ulx) * 4.0f), 12, 12) |                \
                SHIFTL((s32)((float)(uly) * 4.0f), 0, 12),                                     \
            SHIFTL(mode, 24, 2) | SHIFTL((s32)((float)(lrx) * 4.0f), 12, 12) |                 \
                SHIFTL((s32)((float)(lry) * 4.0f), 0, 12))

/* FAKEMATCH: gDPPipeSync written through a halfword-view union initializer, at
 * four sites only. The ROM frame is 0x80 bytes and holds four reload slots,
 * sp+0x1C..0x28, that no instruction references. GCC 2.8.1 store_constructor
 * (expr.c) emits a CLOBBER for every union initializer; combine folds the
 * constant into the store but cannot remove the CLOBBER, so REG_N_SETS stays 1
 * and REG_N_REFS keeps the pseudo (combine.c only zeroes it when the last set
 * goes). Global allocation leaves it without a register and reload's alter_reg
 * gives it a stack slot in pseudo order, right after alpha's (reload1.c). Four
 * such pseudos created between alpha and the strip loop reproduce the frame;
 * the code is unchanged. The four sites are an artificial choice: any four of
 * the seven syncs that do not perturb allocation match. Failed alternatives
 * (each keeps the 0x70 frame or changes code): no-init offset/prevX/prevY/moved,
 * hoisted F6/texrect temporaries, moved declarations (wave 8 e03); pointer or
 * address unions; unions on nonzero second words; structs with constant
 * initializers (no CLOBBER); a one-member u32 union at all 19 sync sites
 * (frame 0xA8, 6164 bytes); this halfword union at all 19 sync sites (frame
 * 0xB8, 6104 bytes) or at all 14 pipe syncs (frame 0xA0, 6104 bytes). */
#define GFX_PIPESYNC_U(pkt)                                                  \
    {                                                                        \
        union { struct { u16 hi, lo; } half; u32 word; } _w = { { 0xE700, 0 } }; \
        GFX_PUT(pkt, _w.word, 0);                                            \
    }

/* Map display state: -1 after a reset, 0 while hidden, 1 once the map is shown. */
s32 D_8013DFD0 = 0;
/* 1: the map is centred on the explored bounds; otherwise it uses the fixed layout. */
s32 D_8013DFD4 = 1;
/* 1: clear the map backdrop with a fill rectangle before drawing it. */
s32 D_8013DFD8 = 0;
/* Front map image (index into the two map buffers). */
s32 D_8013DFDC = 0;
/* Front cursor display list (index into the two cursor buffers). */
s32 D_8013DFE0 = 0;
/* Map origin on screen; initialized to the fixed-layout position. */
s32 D_8013DFE4 = 0x2C;
s32 D_8013DFE8 = 0x23;
/* Explored map bounds in cells (min x, min y, max x, max y); -1 = none yet. */
s32 D_8013DFEC = -1;
s32 D_8013DFF0 = -1;
s32 D_8013DFF4 = -1;
s32 D_8013DFF8 = -1;
/* Render state for the map image (a static libultra display list). */
Gfx D_8013E000[16] = {
    { { 0xE7000000, 0x00000000 } }, /* G_RDPPIPESYNC */
    { { 0xE3000A01, 0x00000000 } }, /* G_SETOTHERMODE_H cycle type 1-cycle */
    { { 0xD9DCF9FA, 0x00000000 } }, /* G_GEOMETRYMODE clear */
    { { 0xE3000F00, 0x00000000 } }, /* G_SETOTHERMODE_H texture LOD tile */
    { { 0xE3001001, 0x00008000 } }, /* G_SETOTHERMODE_H TLUT RGBA16 */
    { { 0xE3000C00, 0x00000000 } }, /* G_SETOTHERMODE_H no perspective */
    { { 0xFC119623, 0xFF2FFFFF } }, /* G_SETCOMBINE */
    { { 0xE3001201, 0x00000000 } }, /* G_SETOTHERMODE_H point filter */
    { { 0xE3001402, 0x00000C00 } }, /* G_SETOTHERMODE_H texture convert filter */
    { { 0xE3001700, 0x00000000 } }, /* G_SETOTHERMODE_H no chroma key */
    { { 0xE2001E01, 0x00000000 } }, /* G_SETOTHERMODE_L no alpha compare */
    { { 0xE3001801, 0x000000C0 } }, /* G_SETOTHERMODE_H color dither disabled */
    { { 0xE3000D01, 0x00000000 } }, /* G_SETOTHERMODE_H texture detail clamp */
    { { 0xE200001C, 0x00504340 } }, /* G_SETOTHERMODE_L render mode */
    { { 0xE7000000, 0x00000000 } }, /* G_RDPPIPESYNC */
    { { 0xDF000000, 0x00000000 } }, /* G_ENDDL */
};
/* RGBA16 palette of the 4-bit map image. */
u16 D_8013E080[16] = {
    0x0000, 0x0887, 0x194D, 0xB5AD, 0xFBF3, 0xB501, 0x0001, 0x003F,
    0xF801, 0xFFFF, 0xCE73, 0x07FF, 0xFFC1, 0x6B5B, 0x8421, 0xA529,
};

/* Pattern tables used by the minimap cell renderer (this file's .data). */
Pat8 D_8013E0A0 = { { 0xF00F0000, 0x0000F00F } };
Pat8 D_8013E0A8 = { { 0x00000FF0, 0x0FF00000 } };
Pat8 D_8013E0B0 = { { 0x0FF0F00F, 0xF00F0FF0 } };
Pat8 D_8013E0B8[2] = {
    { { 0xFFFFFFFF, 0xFFFFFFFF } },
    { { 0xF00F0FF0, 0x0FF0F00F } },
};
Pat8 D_8013E0C8 = { { 0x00000000, 0x00000000 } };
Pat8 D_8013E0D0 = { { 0x77777777, 0x77777777 } };
Pat8 D_8013E0D8 = { { 0xDDDDDDDD, 0xDDDDDDDD } };
Pat8 D_8013E0E0 = { { 0x0BB0BBBB, 0xBBBB0BB0 } };
Pat8 D_8013E0E8 = { { 0x08808888, 0x88880880 } };
Pat8 D_8013E0F0 = { { 0x05505555, 0x55550550 } };
Pat8 D_8013E0F8 = { { 0x03303883, 0x38830330 } };
Pat8 D_8013E100 = { { 0x30030330, 0x03303003 } };
/* Cursor texture data, read by func_8007E0BC. */
u32 D_8013E108[20] = {
    0xCCCC0CC0, 0x00000000, 0xCCCCCCCC, 0x00000000,
    0xCCCCCCCC, 0x00000000, 0xCCCC0CC0, 0x00000000,
    0xCCCC0CC0, 0x00000000, 0xC00CC00C, 0x00000000,
    0xC00CC00C, 0x00000000, 0xCCCC0CC0, 0x00000000,
    0xAAAAAAAA, 0xAAAAAAAA, 0x99999999, 0x99999999,
};
Pat8 D_8013E158 = { { 0x7FF7F00F, 0xF00F7FF7 } };
Pat8 D_8013E160[3] = {
    { { 0x7AA7A00A, 0xA00A7AA7 } },
    { { 0x7AA7AAAA, 0xAAAA7AA7 } },
    { { 0x7AA7A99A, 0xA99A7AA7 } },
};
Pat8 D_8013E178[4] = {
    { { 0x77007F00, 0x00000000 } },
    { { 0x00000000, 0x7F007700 } },
    { { 0x00000000, 0x00F70077 } },
    { { 0x007700F7, 0x00000000 } },
};
Pat8 D_8013E198[4] = {
    { { 0x77007A00, 0x00000000 } },
    { { 0x00000000, 0x7A007700 } },
    { { 0x00000000, 0x00A70077 } },
    { { 0x007700A7, 0x00000000 } },
};
Pat8 D_8013E1B8[4] = {
    { { 0xFF00F000, 0x00000000 } },
    { { 0x00000000, 0xF000FF00 } },
    { { 0x00000000, 0x000F00FF } },
    { { 0x00FF000F, 0x00000000 } },
};
Pat8 D_8013E1D8[8] = {
    { { 0xFFFF0000, 0x00000000 } },
    { { 0xF0000000, 0x00000000 } },
    { { 0xF000F000, 0xF000F000 } },
    { { 0x00000000, 0x0000F000 } },
    { { 0x00000000, 0x0000FFFF } },
    { { 0x00000000, 0x0000000F } },
    { { 0x000F000F, 0x000F000F } },
    { { 0x000F0000, 0x00000000 } },
};
Pat8 D_8013E218[8] = {
    { { 0xA0A00000, 0x00000000 } },
    { { 0xA0000000, 0x00000000 } },
    { { 0xA0000000, 0xA0000000 } },
    { { 0x00000000, 0x00000000 } },
    { { 0x00000000, 0x00000A0A } },
    { { 0x00000000, 0x0000000A } },
    { { 0x0000000A, 0x0000000A } },
    { { 0x00000000, 0x00000000 } },
};
Pat8 D_8013E258 = { { 0xBBBBB00B, 0xB00BBBBB } };
Pat8 D_8013E260 = { { 0xCCCCC00C, 0xC00CCCCC } };
Pat8 D_8013E268 = { { 0x88888008, 0x80088888 } };
/* Neighbour offsets (dx, dy) clockwise from north. */
s32 D_8013E270[16] = {
    0, -1, -1, -1, -1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1,
};

extern float D_8013B7A8, D_8013B7AC, D_8013B7B0, D_8013B7B4;
extern s32 D_801A8070, D_801A8074, D_801A8078, D_801A8098;
extern s32 D_801A807C, D_801A8080, D_801A8084, D_801A8088;
extern u32 D_801A808C[2];    /* scratch pattern for wall cells */
extern u32 *D_801A8094;      /* -> D_801A808C */
extern u32 D_801D9358[][54]; /* map cell flags, 9-cell border */
extern u32 D_801A7F50[72];   /* 36 rows x 64 bits: cell drawn last frame */
/* Two 128-command map buffers and two 16-command cursor buffers. */
extern DisplayCommand D_801A80A0[2][128];
/* ORIGINAL BUG: each cursor display list is a 16-command row; the ROM row
 * stride is 0x80 bytes (sll 7 at 0x8007EB38). func_8007E0BC emits 15 commands
 * into a row when the cursor did not move, but 21 when it moved (the second,
 * fading cursor), five past the row end:
 * - row 0 (D_801A88A0[0]) overwrites the other cursor row, D_801A88A0[1][0..4],
 *   at [0x801A8920, 0x801A8948);
 * - row 1 (D_801A88A0[1]) overwrites the first 40 bytes of the next object,
 *   D_801A89A0, at [0x801A89A0, 0x801A89C8). D_801A89A0 is a 0x200-byte debug
 *   format buffer whose only identified CPU consumer is func_800829D4, a helper
 *   with no caller or data pointer in the ROM.
 * The declaration keeps the ROM-proven [2][16] extent. */
extern DisplayCommand D_801A88A0[2][16];

extern void func_800265E0(void *dst, s32 length);
extern u32 func_8002A9B0(void);
extern s32 func_80041DA8(void);
extern s32 func_80041E50(void);
extern s32 func_80041FF8(void);
extern s32 func_80042990(void);
extern s32 func_800429A0(void);
extern s32 func_800429C0(void);
extern s32 func_800429D0(void);
extern float func_80059658(void);
extern s32 func_8005CB7C(void);
extern s32 func_80061244(void);
extern void func_800624F0(void);
extern s32 func_800627C4(void);
extern s32 func_800627D4(void);
extern void func_8007F090(s32 arg0, s32 arg1);
extern void func_80083568(void);
extern void *func_80083908(s32 index);

void func_8007D8B0(s32 value) {
    D_8013DFD4 = value;
}

void func_8007D8BC(s32 value) {
    D_8013DFD8 = value;
}

void func_8007D8C8(void) {
    D_801A8078 = 1;
}

void func_8007D8DC(s32 value) {
    D_8013DFD0 = value;
}

void func_8007D8E8(void) {
    s32 y;
    s32 x;
    u16 *dstBase;
    u16 *srcBase;

    y = 35;
    dstBase = func_80083908(D_8013DFDC);
    srcBase = func_80083908(D_8013DFDC ^ 1);
    D_801A8094 = D_801A808C;
    D_801A8094[0] = D_801A8094[1] = 0x99999999;
    do {
        x = 57;
        do {
            s32 mx = x + 9;
            s32 my = y + 9;
            u8 *dst = (u8 *)&dstBase[y * 232 + x];
            u32 cell = D_801D9358[mx][my];
            u32 bit = 1 << (x % 32);
            s32 word = y * 2 + (x >> 5);
            u8 *src = NULL; /* overlay pattern, or the previous frame's pixels */
            u8 *base;
            u8 *keep;

            if (cell & 0x800000) {
                if (cell & 0x10000) {
                    base = D_8013E0D0.b;
                    if (cell & 0x80) base = D_8013E0D8.b;
                } else {
                    s32 i;
                    s32 *o;

                    base = D_8013E0C8.b;
                    D_801A8094[0] = D_801A8094[1] = D_8013E0C8.w[0];
                    o = D_8013E270;
                    if (cell & 0xE100) {
                        for (i = 0; i < 8; i++) {
                            u32 nb = D_801D9358[mx + o[0]][my + o[1]];
                            o += 2;
                            if ((nb & 0x18120) == 0x10000) {
                                Pat8 *p;
                                if (cell & 0x2100) {
                                    p = &D_8013E218[i];
                                } else {
                                    p = &D_8013E1D8[i];
                                }
                                D_801A8094[0] |= p->w[0];
                                D_801A8094[1] |= p->w[1];
                            }
                        }
                        if (cell & 0x200) {
                            u32 n = 0;
                            for (i = 1; i < 8; i += 2) {
                                s32 a = (i + 1) % 8 * 2;
                                s32 b = (i + 7) % 8 * 2;
                                /* low flag halves of the two diagonal-adjacent neighbours */
                                u16 v = D_801D9358[mx + D_8013E270[a]][my + D_8013E270[a + 1]]
                                      | D_801D9358[mx + D_8013E270[b]][my + D_8013E270[b + 1]];
                                if (v & 0x400) {
                                    if (!(v & 0xE120)) {
                                        Pat8 *m = &D_8013E1B8[i / 2];
                                        Pat8 *p;
                                        if (cell & 0x2100) {
                                            p = &D_8013E198[i / 2];
                                        } else {
                                            p = &D_8013E178[i / 2];
                                        }
                                        n++;
                                        D_801A8094[0] = (~m->w[0] & D_801A8094[0]) | p->w[0];
                                        D_801A8094[1] = (~m->w[1] & D_801A8094[1]) | p->w[1];
                                    }
                                }
                            }
                            if (n >= 4) {
                                Pat8 *p = &D_8013E158;
                                if (cell & 0x2100) p = &D_8013E160[0];
                                D_801A8094[0] = p->w[0];
                                D_801A8094[1] = p->w[1];
                            }
                        }
                        base = (u8 *)D_801A8094;
                    }
                }
            } else {
                base = NULL;
            }

            if (base != NULL) {
                keep = D_8013E0B8[0].b;
                if (func_8005CB7C() == 0) {
                    if (cell & 0x40000) {
                        src = NULL;
                    } else if (cell & 0x80000) {
                        src = D_8013E0E8.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x4000000) {
                        src = D_8013E0F0.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x200000) {
                        src = D_8013E100.b;
                        keep = D_8013E0B0.b;
                    } else if (cell & 0x400000) {
                        src = D_8013E0E0.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x2000000) {
                        src = D_8013E0F8.b;
                        keep = D_8013E0A0.b;
                    } else if (cell & 0x100000) {
                        src = D_8013E258.b;
                        keep = D_8013E0A8.b;
                    } else if (cell & 0x20000000) {
                        src = D_8013E260.b;
                        keep = D_8013E0A8.b;
                    } else if (cell & 0x10000000) {
                        src = D_8013E268.b;
                        keep = D_8013E0A8.b;
                    }
                }
                if (src != NULL || base != NULL) {
                    if (src == NULL) src = D_8013E0C8.b;
                    if (base == NULL) base = D_8013E0C8.b;
                    dst[0] = src[0] | (base[0] & keep[0]);
                    dst[1] = src[1] | (base[1] & keep[1]);
                    dst[0x74] = src[2] | (base[2] & keep[2]);
                    dst[0x75] = src[3] | (base[3] & keep[3]);
                    dst[0xE8] = src[4] | (base[4] & keep[4]);
                    dst[0xE9] = src[5] | (base[5] & keep[5]);
                    dst[0x15C] = src[6] | (base[6] & keep[6]);
                    dst[0x15D] = src[7] | (base[7] & keep[7]);
                }
                D_801D9358[mx][my] &= ~0x800000;
                D_801A7F50[word] |= bit;
            } else {
                if (D_801A7F50[word] & bit) {
                    src = (u8 *)&srcBase[y * 232 + x];
                    dst[0] = src[0];
                    dst[1] = src[1];
                    dst[0x74] = src[0x74];
                    dst[0x75] = src[0x75];
                    dst[0xE8] = src[0xE8];
                    dst[0xE9] = src[0xE9];
                    dst[0x15C] = src[0x15C];
                    dst[0x15D] = src[0x15D];
                }
                D_801A7F50[word] &= ~bit;
            }
        } while (--x >= 0);
    } while (--y >= 0);
}

s32 func_8007E04C(void) {
    s32 result = 1;

    if ((u8)func_80041FF8()) {
        result = 0;
    } else if ((u8)func_80041E50()) {
        result = 0;
    } else if (func_800627C4() == 1 && func_800627D4() == 0x12) {
        result = 0;
    }
    return result;
}

/* Map cell to pixel offset relative to the centred map origin. */
static inline s32 map_to_screen(s32 cell) {
    return cell * 4 - 36;
}

/* Map overlay: rebuilds the 232x144 4-bit map image display list when the
 * back buffer changed, then draws it scissored plus the (blinking) cursor. */
DisplayCommand *func_8007E0BC(DisplayCommand *gfx) {
    void *images[2];
    u32 alpha;
    s32 offset = 0;
    s32 visible;
    s32 palette = 1;
    s32 width;
    s32 strip;
    s32 col;
    s32 row;
    s32 height;
    DisplayCommand *dl;
    s32 previous;

    if (!func_8007E04C()) {
        return gfx;
    }
    visible = 0;
    if (func_800429D0()) {
        visible = D_801A8074 == 0;
    }
    if (D_801A8070) {
        alpha = 255;
    } else {
        float position = func_80059658();
        float fade;

        if (position < 200.0f) {
            fade = 166.0f;
        } else if (position > 366.0f) {
            fade = 0.0f;
        } else {
            fade = 366.0f - position;
        }
        alpha = 80.0f - (fade / 166.0f) * 40.0f;
    }
    if (D_8013DFD4 == 1) {
        s32 span;

        if (func_80042990() || func_800429A0() || func_800429C0()) {
            func_8007F090(10, 10);
            func_8007F090(65, 43);
        }
        span = (D_8013DFF4 - D_8013DFEC) * 4;
        offset = 73.666664f - span / 3.0f;
        D_8013DFE8 = 141 - D_8013DFF0 * 4 - (((D_8013DFF8 - D_8013DFF0) * 4) >> 1);
        D_8013DFE4 = 194 - offset - D_8013DFEC * 4 - (span >> 1);
    } else {
        D_8013DFE4 = 44;
        D_8013DFE8 = 35;
    }
    images[0] = func_80083908(0);
    images[1] = func_80083908(1);
    if (func_80041DA8() || func_80061244()) {
        D_8013DFD0 = -1;
        return gfx;
    }
    GFX_PIPESYNC_U(gfx++);
    GFX_PUT(gfx++, 0xFA000000, alpha | 0xFFFFFF00);
    if (D_801A8078 == 0) {
        if (!visible && !D_801A8070) {
            return gfx;
        }
        if (D_8013DFD0 <= 0) {
            D_801A8098 = 2;
            D_801A8078 = 1;
            return gfx;
        }
    } else {
        previous = D_8013DFDC;
        D_801A8078 = 0;
        D_8013DFDC = previous ^ 1;
        if (D_801A8098) {
            D_801A8098--;
            func_800265E0(func_80083908(previous), 0x6E00);
            func_80083568();
        }
        func_8007D8E8();
        if (!visible && !D_801A8070) {
            D_8013DFD0 = 0;
            return gfx;
        }
        if (D_8013DFD0 <= 0) {
            D_801A8078 = 1;
            func_800624F0();
            if (++D_8013DFD0 == 0) {
                return gfx;
            }
        }
        D_8013DFD0 = 1;
        dl = D_801A80A0[D_8013DFDC];
        if (D_8013DFD8 == 1) {
            GFX_PIPESYNC_U(dl++);
            GFX_PUT(dl++, 0xE3000A01, 0x00300000);
            GFX_PUT(dl++, 0xD7000000, 0x80008000);
            GFX_PUT(dl++, 0xE200001C, 0);
            GFX_PUT(dl++, 0xF7000000, 0xFFFFFFFF);
            if (D_8013DFD4 == 1) {
                s32 right = (D_8013DFF4 - 10) * 4;
                s32 left = D_8013DFEC * 4 - 44;
                s32 w = right - left;
                s32 bottom = (D_8013DFF8 - 10) * 4;
                s32 top = D_8013DFF0 * 4 - 44;
                s32 h = bottom - top;
                s32 halfW = ((D_8013DFF4 - D_8013DFEC) * 4) >> 1;
                s32 cx = halfW - 158;
                u32 lrx = SHIFTL(w - cx - offset, 14, 10);
                s32 halfH = ((D_8013DFF8 - D_8013DFF0) * 4) >> 1;
                s32 cy = halfH - 105;

                GFX_PUT(dl++, SHIFTL(0xF6, 24, 8) | lrx | SHIFTL(h - cy, 2, 10),
                        SHIFTL(158 - halfW - offset, 14, 10) | SHIFTL(105 - halfH, 2, 10));
            } else {
                GFX_PUT(dl++,
                        SHIFTL(0xF6, 24, 8) | SHIFTL(D_8013DFF4 * 4 + 12, 14, 10) |
                            SHIFTL((D_8013DFF8 + 1) * 4 - 1, 2, 10),
                        SHIFTL(D_8013DFEC * 4 + 8, 14, 10) | SHIFTL(D_8013DFF0 * 4 - 1, 2, 10));
            }
            GFX_PIPESYNC_U(dl++);
        }
        width = 232;
        GFX_PIPESYNC_U(dl++);
        GFX_PUT(dl++, 0xDE000000, (u32)D_8013E000);
        GFX_PUT(dl++, 0xE6000000, 0);
        /* gDPLoadTLUT_pal16(palette, D_8013E080) */
        GFX_PUT(dl++, 0xFD100000, (u32)D_8013E080);
        GFX_PUT(dl++, 0xE8000000, 0);
        GFX_PUT(dl++, SHIFTL(0xF5, 24, 8) | SHIFTL(256 + (palette & 0xF) * 16, 0, 9), 0x07000000);
        GFX_PUT(dl++, 0xE6000000, 0);
        GFX_PUT(dl++, 0xF0000000, 0x0703C000);
        GFX_PUT(dl++, 0xE7000000, 0);
        GFX_PUT(dl++, 0xD7000002, 0);
        GFX_PUT(dl++, SHIFTL(0xFD, 24, 8) | SHIFTL(2, 21, 3) | SHIFTL(1, 19, 2) | SHIFTL((width >> 1) - 1, 0, 12),
                (u32)images[D_8013DFDC]);
        strip = 17;
        col = 0;
        for (row = col; row < 144; row += strip) {
            height = 144 - row;
            if (height > strip) {
                height = strip;
            }
            GFX_PUT(dl++,
                    SHIFTL(0xF5, 24, 8) | SHIFTL(2, 21, 3) | SHIFTL(1, 19, 2) |
                        SHIFTL(((width >> 1) + 7) >> 3, 9, 9),
                    SHIFTL(7, 24, 3) | SHIFTL(palette, 20, 4) | SHIFTL(2, 18, 2) | SHIFTL(2, 8, 2));
            GFX_PUT(dl++, 0xE6000000, 0);
            GFX_PUT(dl++, SHIFTL(0xF4, 24, 8) | SHIFTL(col << 1, 12, 12) | SHIFTL(row << 2, 0, 12),
                    SHIFTL(7, 24, 3) | SHIFTL((width - 1) << 1, 12, 12) |
                        SHIFTL((row + height - 1) << 2, 0, 12));
            GFX_PUT(dl++, 0xE7000000, 0);
            GFX_PUT(dl++,
                    SHIFTL(0xF5, 24, 8) | SHIFTL(2, 21, 3) | SHIFTL(0, 19, 2) |
                        SHIFTL(((width >> 1) + 7) >> 3, 9, 9),
                    SHIFTL(0, 24, 3) | SHIFTL(palette, 20, 4) | SHIFTL(2, 18, 2) | SHIFTL(2, 8, 2));
            GFX_PUT(dl++, 0xF2000000, SHIFTL(width << 2, 12, 12) | SHIFTL(height << 2, 0, 12));
            /* Both layouts emit the same clipped strip (the original keeps two arms). */
            if (D_8013DFD4 == 1) {
                SCIS_TEXRECT(dl++, (D_8013DFE4 + col) << 2, (D_8013DFE8 + row) << 2,
                             (D_8013DFE4 + col + width) << 2, (D_8013DFE8 + row + height) << 2, 0, 0, 0,
                             1 << 10, 1 << 10);
            } else {
                SCIS_TEXRECT(dl++, (D_8013DFE4 + col) << 2, (D_8013DFE8 + row) << 2,
                             (D_8013DFE4 + col + width) << 2, (D_8013DFE8 + row + height) << 2, 0, 0, 0,
                             1 << 10, 1 << 10);
            }
        }
        GFX_PUT(dl++, 0xDF000000, 0);
    }
    {
        DisplayCommand *cursor;
        s32 x, y;
        s32 prevX, prevY;
        s32 blink, prevBlink;
        u32 color, prevColor;
        s32 moved;

        moved = 0;
        GFX_PUT(gfx++, 0xE7000000, 0);
        SET_SCISSOR(gfx++, 0, D_8013B7A8, 38, D_8013B7B0, 177);
        GFX_PUT(gfx++, 0xDE000000, (u32)D_801A80A0[D_8013DFDC]);
        D_8013DFE0 ^= 1;
        cursor = D_801A88A0[D_8013DFE0];
        GFX_PUT(cursor++, 0xE7000000, 0);
        blink = func_8002A9B0() >> 24;
        blink &= 1;
        if (D_8013DFD4 == 1) {
            x = map_to_screen(D_801A807C) + D_8013DFE4;
            y = map_to_screen(D_801A8080) + D_8013DFE8;
        } else {
            x = D_801A807C * 4 + 8;
            y = D_801A8080 * 4 - 1;
        }
        prevY = 0;
        prevX = 0;
        prevBlink = 0;
        if (D_801A807C != D_801A8084 || D_801A8080 != D_801A8088) {
            prevBlink = blink;
            moved++;
            blink = 1;
            if (D_8013DFD4 == 1) {
                prevX = map_to_screen(D_801A8084) + D_8013DFE4;
                prevY = map_to_screen(D_801A8088) + D_8013DFE8;
            } else {
                prevX = D_801A8084 * 4 + 8;
                prevY = D_801A8088 * 4 - 1;
            }
        }
        color = (alpha == 80 && blink) ? 255 : alpha;
        prevColor = (alpha == 80 && prevBlink) ? 255 : alpha;
        GFX_PUT(cursor++, 0xFA000000, SHIFTL(255, 24, 8) | SHIFTL(255, 16, 8) | SHIFTL(255, 8, 8) | SHIFTL(color, 0, 8));
        /* gDPLoadTextureBlock_4b(D_8013E108, CI, 16, 4, palette) */
        GFX_PUT(cursor++, 0xFD500000, (u32)D_8013E108);
        GFX_PUT(cursor++, 0xF5500000, 0x07000000);
        GFX_PUT(cursor++, 0xE6000000, 0);
        GFX_PUT(cursor++, 0xF3000000, 0x0700F800);
        GFX_PUT(cursor++, 0xE7000000, 0);
        GFX_PUT(cursor++, 0xF5400200, SHIFTL(palette, 20, 4));
        GFX_PUT(cursor++, 0xF2000000, 0x0003C00C);
        GFX_PUT(cursor++, 0xE7000000, 0);
        TEXRECT(cursor++, x << 2, y << 2, (x + 4) << 2, (y + 4) << 2, 0, (blink * 4) << 5, 0, 1 << 10, 1 << 10);
        GFX_PUT(cursor++, 0xE7000000, 0);
        /* ORIGINAL BUG: with movement the row gets 21 commands instead of 15,
         * past its 16-command, 0x80-byte extent (stride: sll 7 at 0x8007EB38).
         * Row 0 overwrites the other cursor row, D_801A88A0[1][0..4], at
         * [0x801A8920, 0x801A8948). Row 1 overwrites the first 40 bytes of
         * D_801A89A0 at [0x801A89A0, 0x801A89C8), a debug format buffer whose
         * only identified CPU consumer is the unreferenced func_800829D4. */
        if (moved) {
            GFX_PUT(cursor++, 0xE7000000, 0);
            GFX_PUT(cursor++, 0xFA000000,
                    SHIFTL(255, 24, 8) | SHIFTL(255, 16, 8) | SHIFTL(255, 8, 8) | SHIFTL(prevColor, 0, 8));
            TEXRECT(cursor++, prevX << 2, prevY << 2, (prevX + 4) << 2, (prevY + 4) << 2, 0, (prevBlink * 4) << 5, 0,
                    1 << 10, 1 << 10);
            GFX_PUT(cursor++, 0xE7000000, 0);
        }
        GFX_PUT(cursor++, 0xDF000000, 0);
        GFX_PUT(gfx++, 0xDE000000, (u32)D_801A88A0[D_8013DFE0]);
        GFX_PUT(gfx++, 0xE7000000, 0);
        SET_SCISSOR(gfx++, 0, D_8013B7A8, D_8013B7AC, D_8013B7B0, D_8013B7B4);
    }
    return gfx;
}
