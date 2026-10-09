#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad_00[8]; short adjust_08; short pad_0A; void (*destroy_0C)(void *, s32); } VTable;
typedef struct { u8 pad_00[8]; VTable *vtable_08; u8 flags_0C; signed char x_0D, y_0E; } Item;
extern void *func_800B4D80(Pos *pos);
extern void func_800AD868(Pos *pos);
static inline void set_position(Pos *pos, s32 x, s32 y) { pos->x = x; pos->y = y; }
/* Destructor slot has (receiver, s32 flags), e.g. func_801172AC. */
s32 func_801157E4(Item *self) {
    Pos pos;
    Pos *p = &pos;
    Item *item;
    s32 result;
    if ((self->flags_0C >> 4) & 1) {
        set_position(p, self->x_0D, self->y_0E);
        item = func_800B4D80(p);
        if (item != self) return 0;
        func_800AD868(p);
        if (item) item->vtable_08->destroy_0C((u8 *)item + item->vtable_08->adjust_08, 3);
        result = 1;
    } else result = 0;
    return result;
}
