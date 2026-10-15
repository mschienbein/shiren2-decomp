#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef char *va_list;

/* One 0x2C-byte text window record: the ten-record table D_801A9080 and the
 * current-window pointer D_8013E81C (row_views.h Row_801A9080 is the shared
 * halfword view). Columns and rows count 8-pixel cells. */
typedef struct Row {
    u16 field_00;           /* active */
    u16 field_02;           /* in use */
    u16 field_04;
    u16 field_06;           /* x cell */
    u16 field_08;           /* y cell */
    u16 field_0A;           /* columns */
    u16 field_0C;           /* rows */
    u16 field_0E;
    u16 field_10;           /* dirty */
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    u16 (*spans_1C)[6];     /* per-row colour spans */
    u16 field_20;
    u8 unknown_22[2];
    /* Draw callback, two-form contract: the word is generic function-pointer
     * storage whose form is selected by context_28. A null context means the
     * plain form, registered through func_80081D60 (e.g. func_80054258); a
     * non-null context means the contextual form, registered through
     * func_80081C18 with that context (func_800486A4: func_80048688). The
     * original calls the plain form with no argument (0x8008342C) and the
     * contextual form with the context (0x80083440). */
    void (*draw_24)(void);
    void *context_28;
} Row;

typedef void (*WindowDrawWithContext)(void *context);

/* Text-window state, defined in this order (.data 0x8013E820..0x8013E83B). */
s16 D_8013E820 = -1;        /* current window index (func_800826FC) */
u16 D_8013E822 = 2;         /* span colour, '#c' */
u16 D_8013E824 = 2;         /* colour restored by '#99c' */
u16 D_8013E826 = 2;         /* extra glyph advance, '#b' */
u16 D_8013E828 = 0;         /* glyph remap enable, '#g' */
u16 D_8013E82A = 0;         /* keep-span flag for the next glyph */
u16 D_8013E82C = 1;
u16 D_8013E82E = 0;
s32 D_8013E830 = 0;
u16 D_8013E834 = 6;         /* advance of glyph 0xF0C1, '#w' */
u32 D_8013E838 = 0;         /* glyph y adjustment, low halfword ('#y') */

extern s32 D_8013E814;
extern s32 D_8013E818;
extern Row *D_8013E81C;
extern s32 D_8013E8E4;
extern char D_801A89A0[];
extern u8 D_801A8BA0[][40];
extern u8 *D_801A9050[2];
extern u16 D_801A9058[];
extern Row D_801A9080[10];
extern s16 D_801A9F58;
extern s16 D_801A9F5A;
extern u16 D_801A9F5C;
extern u16 D_801A9F5E;
extern s16 D_801A9F60;
extern s16 D_801A9F62;

extern char *func_80041D70(void);
extern char *func_80048480(u16 id);
extern void func_8005DCEC(s32 id, u8 **data, s32 *width, s32 *height);
extern void func_8005DD80(s32 id, u8 **data, s32 *kind, s32 *count);
extern void func_8005DE08(s32 index, u8 **pixels, s32 *width, s32 *height);
extern s32 func_8005DFE8(unsigned char *str);
extern s32 func_8005E084(u8 *str);
extern s32 func_8005E708(u32 value);
extern s32 func_8005ECF8(char *dst, const char *fmt, va_list args);
extern s32 func_8008269C(s32 v);
extern void func_800826FC(s32 index);
extern char *func_80083C90(char *dst, char *src);
void func_80082998(void);
void func_80082A20(char *text);
void func_80082F3C(s32 offset, u8 *glyph);
void func_80083008(u8 (*glyphs)[8], s32 width, s32 count, s32 mode, s32 keep);
void func_800835AC(u32 index);
void func_800837F4(void);

s32 func_800827BC(void)
{
    return D_8013E820;
}

void func_800827C8(s32 arg0) {
    switch (arg0) {
    case 8: D_8013E822 = 5; break;
    case 10: D_8013E822 = 4; break;
    case 11: D_8013E822 = 5; break;
    case 15: D_8013E822 = 2; break;
    case 22: D_8013E822 = 15; break;
    case 24: D_8013E822 = 13; break;
    case 26: D_8013E822 = 12; break;
    case 31: D_8013E822 = 10; break;
    case 28: D_8013E822 = 11; break;
    default: D_8013E822 = 3; break;
    }
    D_8013E824 = D_8013E822;
}

void func_80082854(s32 first, s32 second) {
    if (first < 0) first = 0;
    D_801A9F58 = first;
    D_801A9F5A = second;
}

void func_80082874(s32 dx, s32 dy)
{
    D_801A9F58 += dx;
    if (D_801A9F58 >= D_8013E81C->field_0A * 8) {
        func_80082998();
    } else if (D_801A9F58 < 0) {
        D_801A9F58 = 0;
    }
    D_801A9F5A += dy;
}

void func_800828FC(void)
{
    Row *rect = D_8013E81C;
    s32 width = rect->field_0A;
    s32 offset = (rect->field_08 * 320 + rect->field_06) * 4;
    s32 rows = rect->field_0C * 8;
    s32 row;
    s32 col;
    u32 *dst;

    for (row = 0; row < rows; row++) {
        dst = (u32 *)(D_801A9050[D_8013E8E4] + offset);
        for (col = 0; col < width; col++) {
            *dst++ = 0;
        }
        offset += 0xA0;
    }
    func_800837F4();
}

void func_80082998(void)
{
    unsigned short a = D_801A9F58;
    unsigned short b = D_801A9F5A;

    D_801A9F58 = 0;
    D_801A9F5E = b;
    D_801A9F5C = a;
    D_801A9F5A = b + 16;
}

void func_800829D4(const char *fmt, ...) {
    va_list args;
    args = (va_list)__builtin_next_arg(fmt);
    func_8005ECF8(D_801A89A0, fmt, args);
    func_80082A20(D_801A89A0);
}

void func_80082A20(char *text) {
    union { char text[128]; u16 pairs[64]; } buffer;
    u8 *pixels;
    s32 width, height;
    s32 centered = 0;
    s32 mode = 0;
    s32 bottom = (D_8013E81C->field_0C + 1) * 8;
    s32 right = D_8013E81C->field_0A * 8;
    s32 pos = 0;
    s32 code, value, sign, i, offset, at;
    char *name;

    while (D_801A9F5A < bottom) {
        code = text[pos++];
        if ((code & 0xF0) == 0xF0) {
            code <<= 8;
            code += text[pos++];
        }
        if (code == 0) break;
        switch (code) {
        case '#':
        case '@':
            sign = 1;
            if (text[pos] == '-') { sign = -1; pos++; }
            value = 0;
            while (func_8005E708(text[pos])) {
                if (value < 999) value = value * 10 + text[pos] - '0';
                pos++;
            }
            value *= sign;
            code = text[pos++];
            if ((code & 0xF0) == 0xF0) {
            code <<= 8;
            code += text[pos++];
        }
            switch (code) {
            case 's': func_80082874(value, 0); break;
            case 't': func_80082874(value - D_801A9F58 % value, 0); break;
            case 'a': mode = value; if (mode == 0) mode = -1; break;
            case 'e': mode = 0; break;
            case 'c':
                if (value == 99) D_8013E822 = D_8013E824;
                else D_8013E822 = value;
                break;
            case 'p': D_801A9F58 = value; break;
            case 'b': D_8013E826 = value; break;
            case 'g': D_8013E828 = value; D_8013E82A = 1; break;
            case 'i':
                func_8005DD80(value, &pixels, &width, &height);
                func_80083008((u8 (*)[8])pixels, width + D_8013E826, height, 0, D_8013E82A);
                break;
            case 'm':
                func_8005DE08(value, &pixels, &width, &height);
                func_80083008((u8 (*)[8])pixels, width + D_8013E826, height, 0, D_8013E82A);
                break;
            case 'w': D_8013E834 = value; break;
            case 'l':
                if (value == 0) D_801A9F58 = (right - func_8005E084((u8 *)&text[pos])) / 2;
                else centered = 1;
                break;
            case 'y': D_8013E838 = value; break;
            case 'n':
                name = func_80041D70();
                code = 0;
                at = 0;
                for (i = 0; i < value; i++) {
                    code = name[at++];
                    if ((code & 0xF0) == 0xF0) {
                        code <<= 8;
                        code += name[at++];
                    }
                    if (code == 0) break;
                }
                if (code != 0) {
                    func_8005DCEC(code, &pixels, &width, &height);
                    func_80083008((u8 (*)[8])pixels, width + D_8013E826, height, 0, D_8013E82A);
                }
                break;
            }
            break;
        case '\n':
            func_80082998();
            break;
        default:
            if (D_8013E828 != 0 && code != 0xF0C1) {
                func_80083C90(buffer.text, func_80048480(0x4DA));
                offset = code - buffer.pairs[0];
                func_80083C90(buffer.text, func_80048480(0x4D8));
                code = buffer.pairs[9] + offset;
            }
            func_8005DCEC(code, &pixels, &width, &height);
            if (code == 0xF0C1) {
                width = D_8013E834;
            } else if (code == 0xF0FC || code == 0xF0F9 || code == 0xF103 || code == 0xF104 || code == 0xF10C) {
                D_8013E838 = 3;
            }
            func_80083008((u8 (*)[8])pixels, width + D_8013E826, height, mode < 0 ? 2 : mode, D_8013E82A);
            D_8013E838 = 0;
            D_8013E82A = 0;
            if (mode > 0) mode--;
            break;
        }
    }
    if (D_8013E820 == func_8008269C(1)) {
        if (D_801A9F58 == 0) {
            D_801A9F60 = D_801A9F5C;
            D_801A9F62 = D_801A9F5E;
        } else {
            D_801A9F60 = D_801A9F58;
            D_801A9F62 = D_801A9F5A;
        }
        if (centered == 1) D_801A9F60 = right / 2;
    }
}

static inline void set_nibble(u8 *dest, s32 index, s32 value) {
    if (value) {
        if (index & 1) *dest = value | (*dest & 0xF0);
        else *dest = (*dest & 0xF) | (value << 4);
    }
}
void func_80082F3C(s32 arg0, u8 *arg1) {
    s32 i = 0;
    u8 *dest = D_801A9050[D_8013E8E4] + arg0 / 2;
    do {
        u8 value = *arg1;
        s32 high = value >> 4;
        u32 low = value & 15;
        set_nibble(dest, arg0, high);
        arg0++;
        if (!(arg0 & 1)) dest++;
        set_nibble(dest, arg0, low);
        arg0++;
        if (!(arg0 & 1)) dest++;
        i += 2;
        arg1++;
    } while (i < 16);
}

void func_80083008(u8 (*glyphs)[8], s32 width, s32 count, s32 mode, s32 keep) {
    s16 y;
    s32 offset;
    s16 height;
    s32 i;
    if (D_801A9F58 + width - D_8013E826 > D_8013E81C->field_0A * 8) {
        if (D_8013E81C->field_00 == 0) return;
        func_80082998();
    }
    y = D_801A9F5A + (u16)D_8013E838;
    offset = (D_8013E81C->field_08 * 320 + D_8013E81C->field_06) * 8 + y * 320 + D_801A9F58;
    height = D_8013E81C->field_0C * 8;
    if (mode != 0 && y + 2 >= 0 && y + 2 < height) {
        s32 n;
        s32 below = offset + 640;
        if (mode == 1) {
            n = width - D_8013E826 - 1;
        } else {
            n = width;
        }
        /* ODD_C: original finite empty loop at ROM PCs 0x80083144..0x80083158.
         * It counts i to n without memory effects; this is not an invented
         * scheduling loop. The blank-glyph call follows the original loop. */
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
            e = &D_8013E81C->spans_1C[row][i];
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
    if (D_801A9F58 >= D_8013E81C->field_0A * 8 && D_8013E81C->field_00 != 0) func_80082998();
}

void func_800833B8(void) {
    s32 index;
    for (index = D_8013E814; index < D_8013E818; index++) {
        Row *window;
        func_800826FC(D_801A9058[index]);
        window = D_8013E81C;
        if (window->field_10 != 0) {
            window->field_10 = 0;
            func_800828FC();
            if (D_8013E81C->context_28 == 0) D_8013E81C->draw_24();
            else ((WindowDrawWithContext)D_8013E81C->draw_24)(D_8013E81C->context_28);
        } else {
            s32 start_y = window->field_08;
            s32 end_y = start_y + window->field_0C;
            s32 start_x = window->field_06;
            s32 end_x = start_x + window->field_0A;
            s32 id = D_801A9058[index];
            s32 y;
            for (y = start_y; y < end_y; y++) {
                s32 x;
                s32 pixel_offset = (y * 320 + start_x) * sizeof(u32);
                for (x = start_x; x < end_x; x++, pixel_offset += sizeof(u32)) {
                    if ((D_801A8BA0[y][x] & 15) == id) {
                        u32 *dest = (u32 *)(D_801A9050[D_8013E8E4] + pixel_offset);
                        u32 *src = (u32 *)(D_801A9050[D_8013E8E4 ^ 1] + pixel_offset);
                        s32 line;
                        for (line = 0; line < 8; line++) {
                            *dest = *src;
                            dest += 40;
                            src += 40;
                        }
                    }
                }
            }
        }
    }
}

void func_80083568(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].field_02 >= 0x40) {
            D_801A9080[i].field_10 = 1;
        }
    }
}

void func_800835AC(u32 index) {
    if (index < 10 && D_801A9080[index].field_02 != 0) {
        D_801A9080[index].field_10 = 1;
    }
}

void func_800835F0(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    if (arg0 < 10U && D_801A9080[arg0].field_02 != 0) {
        D_801A9080[arg0].field_14 = arg1;
        D_801A9080[arg0].field_16 = arg2;
        D_801A9080[arg0].field_18 = arg3;
    }
}

void func_8008364C(u32 index) {
    if (index < 10 && D_801A9080[index].field_02 != 0) {
        D_801A9080[index].field_14 = 0;
    }
}

void func_80083690(u32 index, s32 value) {
    if (index < 10 && D_801A9080[index].field_02 != 0) {
        D_801A9080[index].field_12 = value;
    }
}

void func_800836D4(s32 v) {
    if (v != D_8013E830) {
        s32 r = func_8008269C(1);
        D_8013E830 = v;
        func_800835AC(r);
    }
}

s32 func_80083718(void) { return D_8013E830; }

void func_80083724(s32 value) {
    D_8013E82E = value;
}

/* Both original callers (0x80054048 in func_80053D70, 0x80054308 in func_80054258) test v0 as a
 * full word with no andi: the zero-extended halfword is returned at full width. */
s32 func_80083730(void) {
    return D_8013E82E;
}

s32 func_8008373C(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].field_02 != 0) {
            return 0x40;
        }
    }
    return 0;
}

void func_80083774(s32 idx, u16 *outA, u16 *outB){
    if (outA) *outA = D_801A9080[idx].field_0A;
    if (outB) *outB = D_801A9080[idx].field_0C;
}

void func_800837CC(s32 index, u16 value) {
    D_801A9080[index].field_20 = value;
}

void func_800837F4(void) {
    s32 i = 0;
    Row *obj = D_8013E81C;
    s32 j;
    for (; i < 14; i++) {
        for (j = 0; j < 6; j++) {
            *(short *)((char *)obj->spans_1C + (i * 12 + j * 2)) = 0x2FF;
        }
    }
}

u16 func_80083844(void) { return D_8013E826; }

u16 func_80083850(s32 i){ return D_801A9080[i].field_0A<<3; }

unsigned short func_8008387C(unsigned char *str) { return func_8005DFE8(str) / 8; }

void func_800838A8(unsigned char value)
{
    D_8013E82C = value;
}
