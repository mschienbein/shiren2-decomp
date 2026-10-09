#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u16 active;
    u8 pad2[4];
    u16 x;
    u16 y;
    u16 cols;
    u16 rows;
    u8 padE[0x1C - 0xE];
    u16 (*spans)[6];
} Window;
extern s16 D_801A9F58;
extern s16 D_801A9F5A;
extern Window *D_8013E81C;
extern u16 D_8013E822;
extern u16 D_8013E826;
extern u32 D_8013E838; /* The glyph y adjustment is this word's low halfword. */
void func_80082998(void);
void func_80082F3C(s32 offset, u8 *glyph);
void func_80083008(u8 (*glyphs)[8], s32 width, s32 count, s32 mode, s32 keep) {
    s16 y;
    s32 offset;
    s16 height;
    s32 i;
    if (D_801A9F58 + width - D_8013E826 > D_8013E81C->cols * 8) {
        if (D_8013E81C->active == 0) return;
        func_80082998();
    }
    y = D_801A9F5A + (u16)D_8013E838;
    offset = (D_8013E81C->y * 320 + D_8013E81C->x) * 8 + y * 320 + D_801A9F58;
    height = D_8013E81C->rows * 8;
    if (mode != 0 && y + 2 >= 0 && y + 2 < height) {
        s32 n;
        s32 below = offset + 640;
        if (mode == 1) {
            n = width - D_8013E826 - 1;
        } else {
            n = width;
        }
        for (i = 0; i < n; i++) {
        }
        {
            u8 blank[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0 };
            func_80082F3C(below, blank);
        }
    }
    glyphs += count;
    for (i = 0; i < count; i++) {
        glyphs--;
        if (y >= 0 && y < height) func_80082F3C(offset, *glyphs);
        offset -= 320;
        y--;
        if (offset < 0) break;
    }
    if (D_801A9F5A > 0) {
        for (i = 0; i < 5; i++) {
            s32 row;
            u16 *e;
            s32 start;
            s32 attr;
            s32 end;
            u32 second;
            s32 right;
            if (D_801A9F5A < 12) {
                row = 0;
            } else {
                row = (D_801A9F5A - 12) / 16 + 1;
            }
            e = &D_8013E81C->spans[row][i];
            second = e[0];
            start = second & 0xFF;
            attr = second >> 8;
            right = D_801A9F58 + width;
            second = e[1];
            end = second & 0xFF;
            if (!(D_801A9F58 < start) && start < right && end < right) continue;
            if (attr == D_8013E822) {
                e[0] = (attr << 8) | right;
                e[1] = 0x2FF;
                break;
            }
            if (D_8013E822 == 0x10) {
                if (second != 0x2FF) continue;
                e[0] = D_801A9F58 | (e[0] & 0xFF00);
                e[1] = D_801A9F58 | (D_8013E822 << 8);
                break;
            }
            if (!(start < right) || start == 0xFF) {
                e[0] = (D_8013E822 << 8) | right;
                e[1] = 0x2FF;
                break;
            }
            if (keep) e[0] = D_801A9F58 | (attr << 8);
            e[1] = (D_8013E822 << 8) | right;
            break;
        }
    }
    D_801A9F58 += width;
    if (D_801A9F58 >= D_8013E81C->cols * 8 && D_8013E81C->active != 0) func_80082998();
}
