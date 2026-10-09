#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef u32 size_t;

typedef struct VTable80047624 VTable80047624;

typedef struct {
    char pad0[0x30];
    short width;
    char pad32[0x4C - 0x32];
    VTable80047624 *vtbl;
} Window80047624;

/* D_80152800 slot 0x64 targets func_8009606C; the call supplies self and mode. */
struct VTable80047624 {
    char pad0[0x60];
    short adjust60;
    s32 (*func64)(void *self, s32 index);
};

typedef struct {
    s32 unk0;
    u16 unk4;
    u8 unk6[5];
    u8 unkB[2];
    u8 unkD;
    char padE;
    u8 unkF;
    u8 unk10;
    char pad11[0x1C - 0x11];
    u32 unk1C;
} Entry80047624;

extern const u8 D_8015488C[8];
extern const signed char D_8014A9B0[];
extern const u8 D_8014A9B8[];
extern const char D_8014A9C0[];

extern void func_80048870(void *text, s32 flags);
extern char *func_80048480(u16 id);
extern s32 func_800327C0(char *dst, const char *fmt, ...);
extern void func_800487EC(void *text, s32 column, s32 row, void *string);
extern char *func_80083F34(unsigned char *cursor, s32 length, char *destination);
extern size_t func_80032D70(const char *s);
extern char *func_800A9840(u8 a, u8 b);
extern char *func_80083C90(char *dst, char *src);
extern void func_800CB154(u32 seconds, char *formatDst);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);

/* ODD_C: forwarding helper for one text line. Its pointer parameter gives the
 * text buffer address a pseudo that CSE then reuses for the following calls of
 * the same statement group (the original keeps it in s0 across those calls). */
static inline void draw(Window80047624 *win, s32 row, s32 column, char *text) {
    func_800487EC(win, row, column, text);
}

/* ODD_C: right alignment of a string of `size` bytes; the width argument is
 * read before the length call, as in the original. */
static inline s32 right_align(s32 width, s32 size) {
    return width - size;
}

/* ODD_C: tests one bit of an entry's flag bytes; the pointer parameter makes
 * the flag-byte base a loop invariant as in the original. */
static inline s32 has_flag(const u8 *bits, s32 bit) {
    return bits[bit >> 3] & D_8015488C[bit & 7];
}

void func_80047624(Window80047624 *win, Entry80047624 *entry, s32 index, s32 detailed) {
    char text[0xC8];
    char name[0x10];
    s32 row = index * 4;
    s32 color;
    s32 i;
    s32 count;

    func_80048870(win, win->vtbl->func64((char *)win + win->vtbl->adjust60, 0));
    if (detailed) {
        color = 0x98;
        func_800327C0(text, func_80048480((entry->unkD & D_8015488C[2]) ? 0x503 : 0x502), entry->unk0 + 1);
        draw(win, row, 1, text);
        *func_80083F34(entry->unk6, 4, name) = 0;
        func_800327C0(text, func_80048480(0x453), name);
        draw(win, row, 5, text);
    } else {
        color = 0x80;
        func_800327C0(text, func_80048480((entry->unkD & D_8015488C[2]) ? 0x452 : 0x451), entry->unk0 + 1);
        draw(win, row, 1, text);
        *func_80083F34(entry->unk6, 4, name) = 0;
        func_800327C0(text, func_80048480(0x453), name);
        draw(win, row, 3, text);
    }
    func_800327C0(text, func_80048480(0x455), color, entry->unk4);
    draw(win, row, right_align(win->width, func_80032D70(text) + 1), text);
    func_80083C90(text, func_800A9840(entry->unkF, entry->unk10));
    draw(win, row | 1, 3, text);
    func_800CB154(entry->unk1C, text);
    draw(win, row | 2, 3, text);

    count = 0;
    i = 0;
    for (;;) {
        signed char bit;

        if (i >= 6) {
            break;
        }
        bit = D_8014A9B0[i];
        if (has_flag(entry->unkB, bit)) {
            func_8005EF08(text, D_8014A9C0, D_8014A9B8[i]);
            draw(win, row + 3, count + 3, text);
            count++;
        }
        i++;
    }
}
