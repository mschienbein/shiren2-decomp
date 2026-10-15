#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u32 w0, w1; } Gfx;
typedef struct { Gfx *commands; s32 count; } DisplayList;
extern u8 D_801998B8;
/* ORIGINAL BUG: two 0x3000-byte halves, each holding 0x600 Gfx packets. Emission
 * needs 35+3*N packets, so 501 rendered cells exceed a half; the check is after
 * the writes. A ROM probe with half 1 and the first 501 cells set to 1 first
 * writes outside this 0x6000-byte object at PC 0x8006BE04, address 0x8019F8C0.
 * Later passes reread the overwritten grid, emit 1946 packets and clobber
 * [0x8019F8C0,0x801A0590). The grid D_8019F8C0[26*36] is in that range:
 * func_8006B9B4 reads and rewrites cells (lbu/sb at 0x8006BA7C/0x8006BA98).
 * This renderer's later passes read them; func_8006B910 only zero-fills rectangles.
 * A direct ROM reference scan of all four mapped CPU images finds no code
 * reference to the post-grid bytes [0x8019FC68,0x801A0590).
 * packet-probe.json records all 820 out-of-array word stores and their values;
 * whole-game reachability and other indirect consumers are not established. */
extern Gfx D_801998C0[0xC00];
extern Gfx D_8013CB40[18];
/* Debug-font palettes: two 2-colour RGBA16 arrays and a 0x800-byte CI4 bitmap.
 * The bitmap has 3160 zero and 936 one nibbles; trailing eight zero bytes are
 * outside its extent. G_LOADBLOCK transfers exactly 0x800 bytes.
 * ODD_C: each standard pal16 TLUT packet reads 32 bytes. CBD0..CBF0 covers both
 * palettes and bitmap bytes 0..23; CBD4..CBF4 covers only the second palette
 * and bitmap bytes 0..27. Only entries 0 and 1 are indexed. These are RDP
 * reads, not CPU palette dereferences. ROM uses independent address pairs.
 * Measured single-array, struct and address-taking-accessor forms share the
 * base through CSE and produce 1240 rather than 1248 bytes. Keep the three
 * evidenced, non-overlapping objects; do not enlarge either palette. */
extern u16 D_8013CBD0[2];
extern u16 D_8013CBD4[2];
extern u8 D_8013CBD8[0x800];
extern u8 D_8019F8C0[];
extern const char D_8014C690[];
extern void func_80033048(const char *, ...);

/* gbi-style packet helper: every command gets its own block-scoped packet pointer. */
#define COMMAND(first, second) { Gfx *_g = (cursor++); _g->w0 = (first); _g->w1 = (second); }
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))

void func_8006BAE8(DisplayList *out) {
    Gfx *cursor = D_801998C0 + D_801998B8 * 0x600;
    s32 pass;
    s32 bytes;
    out->commands = cursor;
    /* ORIGINAL BUG: every COMMAND writes before the capacity diagnostic.
     * With half 1 and the 501-cell probe, later passes consume the clobbered
     * grid and the final 1946-packet diagnostic occurs after all 3892 word
     * stores. Preserve the proven 0x3000 stride and the exact effects recorded
     * in wave17-review/astra-o6/packet-probe.json; no reachability claim or
     * nonmatching guard is implied. */
    COMMAND(0xDB060000, 0);
    COMMAND(0xE7000000, 0);
    COMMAND(0xDE000000, (u32)D_8013CB40);
    /* ODD_C: pal16 DMA [0x8013CBD0,0x8013CBF0): both four-byte palettes
     * plus bitmap bytes 0..23. The independent base pair is present in ROM;
     * the measured one-object forms above lose eight instruction bytes. */
    COMMAND(0xFD100000, (u32)D_8013CBD0);
    COMMAND(0xE8000000, 0);
    COMMAND(0xF5000100, 0x07000000);
    COMMAND(0xE6000000, 0);
    COMMAND(0xF0000000, 0x0703C000);
    COMMAND(0xE7000000, 0);
    /* ODD_C: pal16 DMA [0x8013CBD4,0x8013CBF4): the second palette only
     * plus bitmap bytes 0..27. Preserve its independent address pair; the
     * measured single-array/struct/accessor alternatives produce 1240 bytes,
     * rather than the ROM's 1248 bytes. */
    COMMAND(0xFD100000, (u32)D_8013CBD4);
    COMMAND(0xE8000000, 0);
    COMMAND(0xF5000110, 0x07000000);
    COMMAND(0xE6000000, 0);
    COMMAND(0xF0000000, 0x0703C000);
    COMMAND(0xE7000000, 0);
    COMMAND(0xFD500000, (u32)D_8013CBD8);
    COMMAND(0xF5500000, 0x0700C090);
    COMMAND(0xE6000000, 0);
    COMMAND(0xF3000000, 0x073FF040);
    COMMAND(0xE7000000, 0);
    COMMAND(0xF5404000, 0x0000C090);
    COMMAND(0xF2000000, 0x007FC01C);
    for (pass = 0; pass < 4; pass++) {
        s32 y;
        u8 *tile;
        COMMAND(0xE7000000, 0);
        if (pass & 1) {
            COMMAND(0xFA000000, 0xFF404000);
        } else {
            COMMAND(0xFA000000, 0xFFFFFF00);
        }
        if (pass & 2) {
            COMMAND(0xF5404000, 0x0010C090);
        } else {
            COMMAND(0xF5404000, 0x0000C090);
        }
        tile = D_8019F8C0;
        for (y = 0; y < 26; y++) {
            s32 x;
            for (x = 0; x < 36; x++, tile++) {
                if (*tile && (*tile >> 6) == pass) {
                    COMMAND(_SHIFTL(0xE4, 24, 8) | _SHIFTL(x * 32 + 96, 12, 12) | _SHIFTL(y * 32 + 96, 0, 12),
                            _SHIFTL(0, 24, 3) | _SHIFTL(x * 32 + 64, 12, 12) | _SHIFTL(y * 32 + 64, 0, 12));
                    COMMAND(_SHIFTL(0xE1, 24, 8), _SHIFTL((*tile & 0x3F) << 8, 16, 16) | _SHIFTL(0, 0, 16));
                    COMMAND(_SHIFTL(0xF1, 24, 8), _SHIFTL(1 << 10, 16, 16) | _SHIFTL(1 << 10, 0, 16));
                }
            }
        }
    }
    COMMAND(0xDF000000, 0);
    bytes = (u8 *)cursor - (u8 *)D_801998C0;
    if (((bytes - D_801998B8 * 0x3000) >> 3) > 0x600) {
        func_80033048(D_8014C690, (bytes - D_801998B8 * 0x3000) >> 3, 0x600);
    }
    out->count = (bytes - D_801998B8 * 0x3000) >> 3;
    D_801998B8 ^= 1;
}
