#include "common.h"

typedef struct MenuCursor { s32 index; void *records; s32 unknown_08; void *vtable_0C; } MenuCursor;
extern MenuCursor D_801400F0;
static inline void clear_records(MenuCursor *cursor) { cursor->records = 0; }
extern char D_80151350[], D_80149F30[];
void func_800923BC(void) { MenuCursor *obj=&D_801400F0; obj->vtable_0C=D_80151350; clear_records(&D_801400F0); obj->vtable_0C=D_80149F30; }
