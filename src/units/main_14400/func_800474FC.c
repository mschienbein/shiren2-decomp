#include "common.h"
typedef unsigned char u8;
/* D_80152800 slot 0x64 targets func_8009606C; the call supplies self and mode. */
typedef struct { u8 pad_00[0x60]; short adjust_60; short reserved_62; s32 (*flags_64)(void *self, s32 index); } VTable800474FC;
typedef struct {
    u8 pad_00[0x4C]; VTable800474FC *vtable_4C;
    u8 text_50[0x20]; u8 choices_70[8]; s32 selected_78; s32 count_7C;
} Obj800474FC;
extern u8 D_801527E3;
extern void func_80048870(void *text, s32 flags);
extern u8 *func_80083EE4(u8 ch, u8 *dst);
extern void func_800487EC(void *text, s32 column, s32 row, void *string);
extern void func_800487C4(void *text);
static __inline__ s32 menu_flags(Obj800474FC *self, s32 mode) {
    return self->vtable_4C->flags_64((u8 *)self + self->vtable_4C->adjust_60, mode);
}
/* Draws one line per menu choice; the selected choice gets the "#1a"
 * highlight prefix and an empty choice shows the default glyph. */
void func_800474FC(Obj800474FC *self) {
    u8 buffer[16];
    s32 i;
    s32 row = (6 - self->count_7C) / 2;
    func_80048870(self->text_50, menu_flags(self, 0));
    i = 0;
    for (;;) {
        u8 *cursor;
        s32 line;
        if (i >= self->count_7C) break;
        cursor = buffer;
        if (i == self->selected_78) { *cursor++ = '#'; *cursor++ = '1'; *cursor++ = 'a'; }
        if (self->choices_70[i] == 0) {
            cursor = func_80083EE4(D_801527E3, cursor);
        } else {
            cursor = func_80083EE4(self->choices_70[i], cursor);
        }
        *cursor = 0;
        /* ODD_C: the text line (choice row below the title) is formed in two
         * assignments; a twice-set register is not promoted by sched1, which
         * keeps the row sum ahead of the argument moves as in the original. */
        line = row + i;
        line++;
        func_800487EC(self->text_50, 0, line, buffer);
        func_800487C4(self->text_50);
        i++;
    }
}
