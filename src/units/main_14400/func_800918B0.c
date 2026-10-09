#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Cursor-menu method table (root D_80151350, vptr at +0xC; g++ {delta, index, pfn}). */
typedef struct { s16 delta, index; void (*call)(void *self); } OpenSlot;
typedef struct { s16 delta, index; s32 (*call)(void *self); } ActionSlot;
typedef struct { s16 delta, index; s32 (*call)(void *self, s32 direction); } MoveSlot;
typedef struct {
    u8 pad_00[8];
    OpenSlot open;          /* +0x08 */
    ActionSlot confirm;     /* +0x10 */
    ActionSlot cancel;      /* +0x18 */
    ActionSlot action_20;   /* +0x20 */
    ActionSlot action_28;   /* +0x28 */
    MoveSlot move;          /* +0x30 */
} CursorVtbl;

typedef struct MenuCursor {
    s32 index;
    void *records;
    s32 done;
    CursorVtbl *vtable_0C;
} MenuCursor;

extern s32 func_8006D44C(s32 mode, s32 time);


/* Done-flag accessor; the loop test reads it through this inline helper. */
static inline s32 cursor_done(MenuCursor *self) { return self->done; }

/* Cursor-menu event loop (vtable slot +0x40): dispatches buttons until a handler
 * sets done; returns the last handler result. */
s32 func_800918B0(MenuCursor *self)
{
    s32 result = -1;

    self->vtable_0C->open.call((u8 *)self + self->vtable_0C->open.delta);
    self->done = 0;
    while (!cursor_done(self)) {
        switch (func_8006D44C(3, -1)) {
        case 0x33:
            result = self->vtable_0C->confirm.call((u8 *)self + self->vtable_0C->confirm.delta);
            break;
        case 0x34:
            result = self->vtable_0C->cancel.call((u8 *)self + self->vtable_0C->cancel.delta);
            break;
        case 0x37:
            result = self->vtable_0C->action_20.call((u8 *)self + self->vtable_0C->action_20.delta);
            break;
        case 0x35:
            result = self->vtable_0C->action_28.call((u8 *)self + self->vtable_0C->action_28.delta);
            break;
        case 0x38:
            result = self->vtable_0C->move.call((u8 *)self + self->vtable_0C->move.delta, 0);
            break;
        case 0x39:
            result = self->vtable_0C->move.call((u8 *)self + self->vtable_0C->move.delta, 1);
            break;
        case 0x3A:
            result = self->vtable_0C->move.call((u8 *)self + self->vtable_0C->move.delta, 2);
            break;
        case 0x3B:
            result = self->vtable_0C->move.call((u8 *)self + self->vtable_0C->move.delta, 3);
            break;

        }
    }
    return result;
}
