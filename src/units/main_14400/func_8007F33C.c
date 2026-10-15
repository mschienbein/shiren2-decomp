#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

/* 0x8208-byte double-buffered display-list arena; func_8007F130 initializes
   the pixel pointer at +0x8200 after the 4096-command allocation. */
typedef struct {
    u16 palette[0x100];   /* 0x0000 */
    Gfx commands[0x1000]; /* 0x0200 */
    u8 *pixels;           /* 0x8200 */
    u8 pad8204[4];
} RenderBuffer;           /* 0x8208 */

/* One window's 14x6 tile grid (0xA8 bytes). */
typedef struct {
    u16 cells[14][6];
} TileGrid;

/* Ten 0x2C-byte window records at 0x801A9080 (field names follow func_80081C18). */
typedef struct {
    u16 state_00;
    u16 active_02;
    u16 field_04;
    u16 x_06;
    u16 y_08;
    u16 width_0A;
    u16 height_0C;
    u16 order_0E;
    u16 dirty_10;
    u8 pad12[0x1C - 0x12];
    TileGrid *tiles_1C;
    u16 field_20;  /* low 3 bits: half-row split; low 4 bits: grid row offset */
    u8 pad22[0x2C - 0x22];
} Record;

/* Write one display-list command at PKT (callers pass a post-incremented cursor,
   as with the SDK display-list packet macros). */
#define GFX(pkt, hi, lo)                   \
    {                                      \
        Gfx *_g = (pkt);                   \
        _g->w0 = (hi);                     \
        _g->w1 = (lo);                     \
    }

/* Load a 16-entry RGBA16 palette into TMEM palette PAL (gDPLoadTLUT_pal16). */
#define GFX_LOAD_TLUT_PAL16(pkt, pal, dram)                              \
    {                                                                    \
        GFX(pkt, 0xFD100000, (u32)(dram));                               \
        GFX(pkt, 0xE8000000, 0);                                         \
        GFX(pkt, 0xF5000000 | (0x100 + ((pal) & 15) * 16), 0x07000000);  \
        GFX(pkt, 0xE6000000, 0);                                         \
        GFX(pkt, 0xF0000000, 0x0703C000);                                \
        GFX(pkt, 0xE7000000, 0);                                         \
    }

/* Record renderer state (.data 0x8013E8DC..0x8013E8F0) shared by this file's two functions. */
s32 D_8013E8DC = 1;
s32 D_8013E8E0 = 1;
s32 D_8013E8E4 = 0;
/* Resources loaded by func_8007F2D8 (func_8006ABC4 results). */
void *D_8013E8E8 = 0;
void *D_8013E8EC = 0;
u8 D_8013E8F0 = 0;
extern s32 D_8013E814;
extern s32 D_8013E818;
extern u16 D_8013E82C;
extern u8 D_8013E83C[][3];   /* RGB per window style */
extern u16 D_8013E86C[2][16]; /* two 16-entry TLUT rows, 0x8013E86C..0x8013E8AC */
extern u8 D_8013E2B0[];
extern u8 D_8013E6D0[];
extern u8 D_801A8BA0[30][40];
extern u8 *D_801A9050[2];
extern u16 D_801A9058[10];
extern u16 D_801A906C[10];
extern Record D_801A9080[10];
extern RenderBuffer D_801A9F68[2];
extern u8 D_5000010[];
extern u8 D_5000090[];
extern u8 D_5000188[];
extern u8 D_5000230[];

u32 func_800340F0(void *addr);
void func_80034720(void *p, s32 len);
s32 func_80041D94(void);
s32 func_80041E50(void);
s32 func_8007D388(void);
void func_80080CD0(Gfx **gdl, s32 unused); /* caller passes 2 in a1; callee ignores it */
void func_80080E04(Gfx **gfx, s32 id, s32 d, s32 e);
void func_80081040(Gfx **display, s32 id, s32 unused); /* caller passes 0 in a2; callee ignores it */
void func_8008138C(Gfx **gfxp, s32 id, s32 unused, s32 colorIndex); /* caller passes 0 in a2 */
void func_80081798(void);
void func_80081964(void);
void func_800833B8(void);

/* Fill-rectangle packet (gDPFillRectangle) shared by the background and
   highlight paths.  As a function its arguments are evaluated once, which
   the ROM's row*8 then +8 sequence at 8007F8AC..8007F8C4 reflects; the
   equivalent macro measured 57 differing words. */
static inline void fill_rectangle(Gfx *packet, s32 left, s32 top, s32 right, s32 bottom) {
    packet->w0 = 0xF6000000 | ((right & 0x3FF) << 14) | ((bottom & 0x3FF) << 2);
    packet->w1 = ((left & 0x3FF) << 14) | ((top & 0x3FF) << 2);
}

void func_8007F33C(void) {
    D_8013E8E8 = 0;
    D_8013E8EC = 0;
}

Gfx *func_8007F350(Gfx *list) {
    Gfx *commands;
    RenderBuffer *buffer;
    s32 index;
    s32 pass;
    s32 mask;
    s32 x, y, span_end;
    s32 x_origin, y_origin, x_limit, y_limit;
    s32 bottom, row_top;
    s32 x_px;
    s32 split;
    Record *window;
    s32 left, right, end, top;
    s32 mode, run, boundary, style, alpha, tile;
    u32 row; /* ODD_C: also holds the window state for the open test; see below */
    u16 run_value;

    if (D_8013E8E8 == 0 || D_8013E8EC == 0) {
        return list;
    }
    if (D_8013E8E0 == 0) {
        if (D_8013E818 <= D_8013E814) {
            return list;
        }
    } else {
        func_80081798();
        func_80081964();
        D_8013E8DC = 1;
    }
    D_8013E8E0 = 0;
    for (index = D_8013E814; index < D_8013E818; index++) {
        if (D_801A9080[D_801A9058[index]].dirty_10 != 0) {
            func_800833B8();
            D_8013E8DC = 1;
            break;
        }
    }
    if (D_8013E818 <= D_8013E814) {
        return list;
    }
    GFX(list++, 0xE7000000, 0);
    GFX(list++, 0xDB060014, func_800340F0(D_8013E8E8));
    GFX(list++, 0xDB060018, func_800340F0(D_8013E8EC));
    GFX(list++, 0xDB060020, func_800340F0(D_801A9050));

    if (D_8013E8DC == 0) {
        buffer = &D_801A9F68[D_8013E8E4 ^ 1];
        GFX(list++, 0xDB06001C, func_800340F0(buffer));
        GFX(list++, 0xDE000000, (u32)buffer->commands);
        if (D_8013E8F0++ >= 8) {
            GFX(list++, 0xE7000000, 0);
            GFX(list++, 0xE3001001, 0xC000);
            GFX(list++, 0xE200001C, 0x504240);
            GFX(list++, 0xFCFF97FF, 0xFF2CFE7F);
            GFX(list++, 0xDE000000, (u32)D_5000188);
            func_80080CD0(&list, 2);
            D_8013E8F0 %= 20;
        }
        return list;
    }

    D_8013E8DC = 0;
    buffer = &D_801A9F68[D_8013E8E4];
    commands = buffer->commands;
    GFX(list++, 0xDB06001C, func_800340F0(buffer));
    GFX(commands++, 0xDE000000, (u32)D_5000010);
    /* ODD_C: original redundant prewrites, overwritten by the loop. ROM evidence:
       lhu of D_8013E86C[1][0] at 0x8007F70C, sh to palette[16] at 0x8007F71C and
       palette[0] at 0x8007F720, before the copy loop at 0x8007F724. */
    buffer->palette[0] = buffer->palette[16] = D_8013E86C[1][0];
    for (index = 0; index < 16; index++) {
        buffer->palette[index] = D_8013E86C[0][index];
        buffer->palette[index + 16] = D_8013E86C[1][index];
    }
    GFX_LOAD_TLUT_PAL16(commands++, 2, func_800340F0(buffer));
    GFX_LOAD_TLUT_PAL16(commands++, 3, func_800340F0(&buffer->palette[16]));
    GFX(commands++, 0xE7000000, 0);

    if (D_8013E82C != 0) {
        s32 row_y, col, next;

        for (row_y = 0; row_y < 30; row_y++) {
            s32 top_px = row_y * 8;

            for (col = 0; col < 40; col++) {
                if ((D_801A8BA0[row_y][col] & 15) == 15) {
                    continue;
                }
                for (next = col + 1; next < 40; next++) {
                    if ((D_801A8BA0[row_y][next] & 15) == 15) {
                        break;
                    }
                }
                fill_rectangle(commands++, col * 8 + 4, top_px, next * 8 - 4, top_px + 8);
                col = next;
            }
        }
    }
    GFX(commands++, 0xE7000000, 0);
    if ((u8)func_80041E50()) {
        mask = 15;
    } else if (func_80041D94() == 0) {
        mask = 15;
    } else if (func_8007D388() != 0) {
        mask = 8;
    } else {
        mask = 15;
    }
    GFX(commands++, 0xDE000000, (u32)D_5000090);
    func_80080E04(&commands, 0x40, 0, mask);
    GFX(commands++, 0xD7000000, 0);

    for (index = 0; index < 10; index++) {
        window = &D_801A9080[index];
        /* ODD_C: the state is read into the unsigned scratch `row` (reused below
           for the tile-grid row), which makes it a function-wide pseudo that
           global allocation places in v1 and keeps untied from the sltiu
           result in v0 (8007FA74..8007FA78). A block-local temporary or a
           direct field test is tied by local-alloc and leaves 2 words; an s32
           scratch gives slti (1 word). */
        row = window->active_02;
        if (row < 0x40) {
            continue;
        }
        x_origin = window->x_06;
        y_origin = window->y_08;
        x_limit = x_origin + window->width_0A;
        y_limit = y_origin + window->height_0C;
        x_px = x_origin * 8;
        for (y = y_origin; y < y_limit; y++) {
            for (x = x_origin; x < x_limit; x++) {
                if ((D_801A8BA0[y][x] & 15) != index) {
                    continue;
                }
                for (span_end = x + 1; span_end < x_limit; span_end++) {
                    if ((D_801A8BA0[y][span_end] & 15) != index) {
                        break;
                    }
                }
                row_top = y * 8;
                split = (window->field_20 & 7) != 0;
                for (pass = 0; pass < split + 1; pass++) {
                    left = x * 8;
                    end = right = span_end * 8;
                    do {
                        mode = window->field_20 & 15;
                        if (mode == 0) {
                            row = (pass + 4 + (y - y_origin) * 2) / 4;
                        } else {
                            row = (pass + (y - y_origin) * 2 + mode / 4) / 4;
                        }
                        for (run = 0; run < 6; run++) {
                            run_value = window->tiles_1C->cells[row][run];
                            boundary = run_value & 0xFF;
                            style = run_value >> 8;
                            if (boundary == 0xFF) {
                                right = end;
                                break;
                            }
                            if (left - x_px < boundary) {
                                right = x_px + boundary;
                                if (right > end) {
                                    right = end;
                                }
                                break;
                            }
                        }
                        top = row_top;
                        if (split == 0) {
                            bottom = top + 8;
                        } else {
                            bottom = top + 4;
                        }
                        if (style < 16) {
                            if (style >= 8 && style < 15) {
                                GFX(commands++, 0xFCFFFFFF, 0xFFFDF6FB);
                                GFX(commands++, 0xFA000000, 0x305064FF);
                                fill_rectangle(commands++, left, top, right,
                                               bottom - ((y_origin & 1) ^ (y & 1)) * 2);
                                GFX(commands++, 0xE7000000, 0);
                                GFX(commands++, 0xFCFFFFFF, 0xFFFCF279);
                                style -= 8;
                            }
                            if (style == 15) {
                                alpha = D_801A906C[index] != 0 ? 0xA0 : 0xFF;
                            } else if (D_801A906C[index] != 0 || style >= 8) {
                                alpha = 0xA0;
                            } else {
                                alpha = 0xFF;
                            }
                            GFX(commands++, 0xE3001001, 0);
                            GFX(commands++, 0xFA000000,
                                    (D_8013E83C[style][0] << 24) | (D_8013E83C[style][1] << 16) |
                                        (D_8013E83C[style][2] << 8) | alpha);
                            GFX(commands++, 0xE200001C, 0x504240);
                            GFX(commands++, 0xFCFF97FF, 0xFF2DFEFF);
                            GFX(commands++, 0xFD900000, (u32)(D_801A9050[D_8013E8E4] + top * 160));
                            GFX(commands++, 0xF5900000, 0x07000000);
                            GFX(commands++, 0xE6000000, 0);
                            GFX(commands++, 0xF3000000, 0x0727F067);
                            GFX(commands++, 0xE7000000, 0);
                            GFX(commands++, 0xF5802800, (style & 15) << 20);
                            GFX(commands++, 0xF2000000, 0x4FC01C);
                        } else {
                            s32 opacity = D_801A906C[index] != 0 ? 0xA0 : 0xFF;

                            if (style == 16) {
                                tile = 2;
                            } else if (style < 50) {
                                GFX_LOAD_TLUT_PAL16(commands++, 3, D_8013E2B0 + (style - 17) * 32);
                                tile = 3;
                            } else {
                                GFX_LOAD_TLUT_PAL16(commands++, 3, D_8013E6D0 + (style - 50) * 32);
                                tile = 3;
                            }
                            GFX(commands++, 0xE3001001, 0);
                            GFX(commands++, 0xFA000000, opacity | 0xFFFFFF00);
                            GFX(commands++, 0xE3001001, 0x8000);
                            GFX(commands++, 0xE200001C, 0x504240);
                            GFX(commands++, 0xFCFF97FF, 0xFF2CFE7F);
                            GFX(commands++, 0xFD500000, (u32)(D_801A9050[D_8013E8E4] + top * 160));
                            GFX(commands++, 0xF5500000, 0x07000000);
                            GFX(commands++, 0xE6000000, 0);
                            GFX(commands++, 0xF3000000, 0x0727F067);
                            GFX(commands++, 0xE7000000, 0);
                            GFX(commands++, 0xF5402800, tile << 20);
                            GFX(commands++, 0xF2000000, 0x4FC01C);
                        }
                        GFX(commands++, 0xE7000000, 0);
                        GFX(commands++, 0xE4000000 | (((right * 4) & 0xFFF) << 12) | ((bottom * 4) & 0xFFF),
                                (((left * 4) & 0xFFF) << 12) | ((top * 4) & 0xFFF));
                        GFX(commands++, 0xE1000000, left << 21);
                        GFX(commands++, 0xF1000000, 0x04000400);
                        if (right != end) {
                            left = right;
                        }
                        GFX(commands++, 0xE7000000, 0);
                    } while (right != end);
                    row_top += 4;
                }
                x = span_end;
            }
        }
    }
    GFX(commands++, 0xE7000000, 0);
    GFX(commands++, 0xE3001001, 0xC000);
    GFX(commands++, 0xE200001C, 0x504240);
    GFX(commands++, 0xFCFF97FF, 0xFF2CFE7F);
    GFX(commands++, 0xDE000000, (u32)D_5000188);
    func_80081040(&commands, 0x40, 0);
    GFX(commands++, 0xDE000000, (u32)D_5000230);
    func_8008138C(&commands, 0x40, 0, mask);
    GFX(commands++, 0xE3001001, 0x8000);
    GFX(commands++, 0xD7000000, 0);
    GFX(commands++, 0xDF000000, 0);
    GFX(list++, 0xDE000000, (u32)buffer->commands);
    func_80034720(buffer, sizeof(RenderBuffer));
    func_80034720(D_801A9050, sizeof(D_801A9050));
    D_8013E8E4 ^= 1;
    return list;
}
