#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x, y; } Pair;
/* Widget family (root D_80151E38) slots: +0x5C void (void *self, s32 index, char *buf);
 * +0x64 s32 (void *self, s32 index). */
typedef struct { s16 delta, index; void (*call)(void *self, s32 index, char *buf); } TextSlot;
typedef struct { s16 delta, index; s32 (*call)(void *self, s32 index); } ColorSlot;
typedef struct { u8 pad_00[0x58]; TextSlot text; ColorSlot color; } VTable;
typedef struct {
    u8 pad_00[0x20]; s32 width_20, height_24;
    u8 pad_28[0x24]; VTable *vtable_4C;
    void *rows_50; s32 spacing_54;
} Menu;
extern void func_80048870(Menu *text, s32 flags);
extern void func_800487EC(Menu *text, s32 style, s32 flags, char *str);

static inline s32 has_row(Menu *self, s32 y) {
    return y < self->height_24;
}

static inline s32 cell_index(Menu *self, Pair *pos) {
    return pos->x + self->width_20 * pos->y;
}

static inline s32 cell_color(Menu *self, Pair *pos) {
    return self->vtable_4C->color.call(
        (u8 *)self + self->vtable_4C->color.delta, cell_index(self, pos));
}

static inline void cell_text(Menu *self, Pair *pos, char *text) {
    self->vtable_4C->text.call((u8 *)self + self->vtable_4C->text.delta,
        cell_index(self, pos), text);
}

void func_80097318(Menu *self) {
    Pair pos;
    for (pos.y = 0; ; pos.y++) {
        if (!has_row(self, pos.y)) break;
        for (pos.x = 0; ; pos.x++) {
            char text[0x400];
            if (pos.x >= self->width_20) break;
            func_80048870(self, cell_color(self, &pos));
            cell_text(self, &pos, text);
            func_800487EC(self, pos.x, pos.y * (self->spacing_54 + 1) + 1, text);
        }
    }
}
