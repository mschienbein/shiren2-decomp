#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct { s32 x, y; } Pair;

/* Widget family (root D_80151E38) slot +0x5C: void (void *self, s32 index, char *buf),
 * entered through the g++ {delta, index, pfn} entry at +0x58. */
typedef struct { s16 delta, index; void (*call)(void *self, s32 index, char *buf); } TextSlot;
typedef struct { u8 pad_00[0x58]; TextSlot text; } VTable;

typedef struct {
    u8 pad_00[0x20];
    s32 width_20;
    u8 pad_24[0x28];
    VTable *vtable_4C;
} Menu;

/* Writes the text of grid cell pos (flattened row-major) into text. The original never
 * touches a2: the caller's text buffer is forwarded unchanged to the slot, which
 * consumes it as its third argument. */
void func_80096010(Menu *self, Pair *pos, char *text)
{
    self->vtable_4C->text.call((u8 *)self + self->vtable_4C->text.delta,
        pos->x + self->width_20 * pos->y, text);
}
