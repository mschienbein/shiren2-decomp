#include "common.h"
typedef unsigned char u8;
/* D_80152800 slot 0x64 targets func_8009606C; the call supplies self and mode. */
typedef struct { u8 pad_00[0x60]; short adjust_60; short reserved_62; s32 (*flags_64)(void *self, s32 index); } VTable80046F48;
typedef struct {
    u8 pad_00[0x4C];
    VTable80046F48 *vtable_4C;
    char *title_50;
    char *footer_54;
    s32 value_58;
    u8 pad_5C[8];
    s32 count_64;
    s32 cursor_68;
} Menu80046F48;
/* Digit buffer filled by "%*d" (two characters per entry) and the line buffer. */
extern u8 D_80160B70[];
extern u8 D_80160B82[];
extern void func_80048870(void *text, s32 flags);
extern void func_800487EC(void *text, s32 column, s32 row, void *string);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
extern void func_80082A20(void *text);
static __inline__ s32 menu_flags(Menu80046F48 *self, s32 mode) {
    return self->vtable_4C->flags_64((u8 *)self + self->vtable_4C->adjust_60, mode);
}
/* Prints the title, then one two-digit entry per line; the entry under the
 * cursor (counted from the end) gets the "#1a" highlight prefix. */
void func_80046F48(Menu80046F48 *self) {
    s32 i;
    s32 pos;
    u8 *buf;

    func_80048870(self, menu_flags(self, 0));
    func_800487EC(self, 0, 0, self->title_50);
    func_8005EF08((char *)D_80160B70, "%*d", self->count_64, self->value_58);
    i = 0;
    while (1) {
        if (!(i < self->count_64)) {
            break;
        }
        pos = 0;
        if (self->count_64 - i - 1 == self->cursor_68) {
            D_80160B82[pos++] = '#';
            D_80160B82[pos++] = '1';
            D_80160B82[pos++] = 'a';
        }
        buf = D_80160B82;
        D_80160B82[pos] = D_80160B70[i * 2];
        pos++;
        D_80160B82[pos] = D_80160B70[i * 2 + 1];
        D_80160B82[pos + 1] = 0;
        func_80082A20(buf);
        i++;
    }
    func_80082A20(self->footer_54);
}
