#include "common.h"

typedef unsigned char u8;
typedef struct PaletteView Text80051860;
typedef struct PaletteView {
    u8 pad_00[0x84];
    u8 *characters_84;
    s32 flag_88;
} PaletteView;
extern void func_80048870(Text80051860 *text, s32 flags);
extern char *func_80048480(unsigned short id);
extern void func_800487EC(Text80051860 *text, s32 row, s32 column, char *string);
extern u8 *func_80083EE4(u8 character, u8 *output);
extern const u8 D_80152890[4];

void func_8009B8F8(void *object)
{
    PaletteView *palette = object;
    u8 column;
    s32 row, offset;
    char glyph[4];
    func_80048870(palette, 0x38000000);
    column = D_80152890[0];
    func_800487EC(palette, 0, column, func_80048480(0x293));
    if (palette->flag_88) {
        column = D_80152890[1];
        func_800487EC(palette, 0, column, func_80048480(0x294));
    }
    column = D_80152890[2];
    func_800487EC(palette, 0, column, func_80048480(0x295));
    column = D_80152890[3];
    func_800487EC(palette, 0, column, func_80048480(0x296));
    func_80048870(palette, 0x78000000);
    for (row = 0, offset = 0; ; offset += 10, ++row) {
        s32 i, x;
        if (row >= 6)
            break;
        i = 0;
        x = 1;
        while (1) {
            u8 *end;
            if (i >= 10)
                break;
            end = func_80083EE4(palette->characters_84[offset + i], (u8 *)glyph);
            ++i;
            *end = 0;
            func_800487EC(palette, row + 1, x, glyph);
            x += 2;
        }
    }
}
