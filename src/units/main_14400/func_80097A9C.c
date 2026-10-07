#include "common.h"

/* Cursor position {column, row}. */
typedef struct { s32 x; s32 y; } Pos;

/* Widget vtable slot 16 (+0x80 this-adjust, +0x84 method): moves the cursor to a
 * position. This class has no cursor; self and pos are passed by the slot contract and
 * unused. */
void func_80097A9C(void *self, Pos *pos) {
}
