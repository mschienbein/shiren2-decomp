#include "common.h"

/* Cursor position {column, row}. */
typedef struct { s32 x; s32 y; } Pos;

/* Widget vtable slot 15 (+0x78 this-adjust, +0x7C method): value of the item at a cursor
 * position. This class reports 0 for every position; self and pos are passed by the slot
 * contract and unused. */
s32 func_8009BA48(void *self, Pos *pos)
{
    return 0;
}
