#include "func_801E67D0_135320/types.h"

void func_801E67D0_135320(u16 *workspace) {
    /* KSEG1 cart-ROM mapping: the first packed word precedes this cursor. */
    const u32 *source = (const u32 *)0xB0000B74;
    s32 bit = 0;
    u32 word = source[-1];
    s32 i;
    s32 glyph;
    s32 row;
    u16 *out;
    D_801E8284_136DD4 = workspace;
    for (i = 0x86F; i >= 0; i--) {
        *workspace++ = 0;
    }
    for (i = 0; i < sizeof(D_801E7E40_136990) / sizeof(D_801E7E40_136990[0]); i++) {
        if (i == 3) {
            source = D_801E78F0_136440;
            bit = 0;
            word = *source++;
        }
        out = &D_801E8284_136DD4[D_801E7E40_136990[i].first * 16 + 2];
        for (glyph = 0; glyph < D_801E7E40_136990[i].count; glyph++) {
            for (row = 0; row < 14; row++) {
                bit += 13;
                if (bit & ~31) {
                    bit &= 31;
                    *out = (word << bit) & 0x1FFF;
                    word = *source++;
                    if (bit == 0) {
                        goto next_row;
                    }
                }
                *out |= (word >> (32 - bit)) & 0x1FFF;
next_row:
                out++;
            }
            out += 2;
            bit += 2;
        }
    }
    for (i = 0x460; i < 0x5F0; i++) {
        D_801E8284_136DD4[i + 0x270] = D_801E8284_136DD4[i];
    }
    for (i = 0x5A0; i < 0x5F0; i++) {
        D_801E8284_136DD4[i + 0x220] = D_801E8284_136DD4[i];
    }
    row = 0;
    out = &D_801E8284_136DD4[(0x7C - 15) * 16];
    for (i = 0x7C; i < 0x95; i++) {
        if (i == 0x90) {
            row = 8;
        }
        for (glyph = 0; glyph < 4; glyph++) {
            out[glyph] = (out[glyph] | (s8)D_801E7E48_136998[glyph + row]) &
                (s8)D_801E7E48_136998[glyph + row + 4];
        }
        out += 16;
    }
}
