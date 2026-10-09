#include "common.h"

typedef struct MenuCursor { s32 index; void *records; s32 unknown_08; void *vtable_0C; } MenuCursor;
extern MenuCursor D_80140130;
static inline void clear_records(MenuCursor *cursor) { cursor->records = 0; }
extern char D_80151350[], D_80149EE0[];
void func_800925AC(void) { MenuCursor *p = &D_80140130; p->vtable_0C = D_80151350; clear_records(&D_80140130); p->vtable_0C = D_80149EE0; }
