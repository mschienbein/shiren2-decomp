#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 category, kind, flags, field_03, count, pad_05[7];
    u8 appearance, appearance_flags, pad_0E[2], count_10, pad_11[0x17];
    u8 field_28;
} Item;
u8 D_801397B0[8] = {32, 40, 28, 24, 36, 0, 0, 0};
u8 D_801397B8[8] = {40, 42, 39, 38, 41, 0, 0, 0};
extern const char D_8014B51C[];
extern char *func_80048480(unsigned short), *func_80083C90(char *, char *), *func_800AE674(Item *);
extern s32 func_8005EF30(char *, const char *, ...), func_80083DC4(s32);
extern u8 func_800AC1AC(u8);
extern s32 func_800AE598(Item *), func_800AE9AC(Item *, s32, s32), func_800AEC2C(Item *);
char *func_800514F0(Item *item, char *output, s32 mode, s32 x, s32 line, s32 price_mode, s32 flags) {
    char format[0x68], prefix[0x18], suffix[0x18];
    s32 kind = item->kind, category = item->category, count;
    unsigned short icon, style;
    if (kind == 0xF2) {
        s32 appearance_known = (item->appearance_flags >> 7) ^ 1;
        if (appearance_known) {
            kind = item->appearance;
            category = func_800AC1AC(kind);
        }
    }
    /* Special category icons fall back to the ordinary icon/style pair through
     * the shared `normal` arm (cases 3, 4 and 6 when func_800AE598 is not 1). */
    switch ((u32)category - 3) {
    case 0: {
        s32 ordinary = func_800AE598(item);
        ordinary ^= 1;
        if (ordinary) goto normal;
        icon = 0x38; style = 0x2E;
        break;
    }
    case 1: {
        s32 ordinary = func_800AE598(item);
        ordinary ^= 1;
        if (ordinary) goto normal;
        icon = 0x39; style = 0x2F;
        break;
    }
    case 3: {
        s32 ordinary = func_800AE598(item);
        ordinary ^= 1;
        if (ordinary) goto normal;
        icon = 0x3A; style = 0x30;
        break;
    }
    case 14:
        icon = D_801397B0[kind - 0xE9]; style = D_801397B8[kind - 0xE9]; break;
    case 16:
        switch (kind) { case 0xEF: icon = 0x12; break; case 0xF0: icon = 0x10; break; case 0xF1: default: icon = 0x14; break; }
        style = icon + 0x11; break;
    case 6:
        icon = 0x3B;
        if (kind == 0xB0) { style = 0x31; break; }
        if (!item->field_28) { icon = 0x37; style = 0x2D; break; }
        /* fall through */
    default:
    normal:
        icon = category + 1; style = category + 0x12; break;
    }
    {
    s32 value = 0;
    if (price_mode) {
        if (price_mode == 2) value = func_800AEC2C(item);
        else value = func_800AE9AC(item, 2, flags);
        if (value < 0) value = 0;
        if (value > 9999999) value = 9999999;
    }
    func_80083C90(format, func_80048480(value > 0 ? 0x4F1 : 0x4F2));
    if (item->flags & 4) func_8005EF30(prefix, func_80048480(0x4F3), mode);
    else prefix[0] = 0;
    if (func_800AE598(item)) func_8005EF30(suffix, func_80048480(0x4F4), mode);
    else suffix[0] = 0;
    if (line < 0) { count = 1; line = 0; }
    else if (kind == 0xF1) count = item->count_10;
    else count = item->count;
    if (line < count) {
        if (line == 0) {
            char *name;
            s32 offset;
            if (count >= 2) icon++;
            name = func_800AE674(item);
            offset = (7 - func_80083DC4(value)) * 6 - 0x2E;
            func_8005EF30(output, format, mode, style & 0xFFFF, icon & 0xFFFF, prefix, suffix, name, x + offset, value);
            return output;
        }
        if (line == count - 1) icon += 2;
        else icon += 3;
        func_8005EF30(output, D_8014B51C, style & 0xFFFF, icon & 0xFFFF);
    } else *output = 0;
    return output;
    }
}
