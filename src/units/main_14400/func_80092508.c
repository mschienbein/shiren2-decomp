#include "common.h"

typedef unsigned char u8;

typedef struct MenuCursor { s32 index; void *records; s32 unknown_08; const void *vtable_0C; } MenuCursor;
extern MenuCursor D_80140100;
static inline void clear_records(MenuCursor *cursor) { cursor->records = 0; }
extern const s32 D_80151350[20]; /* Complete 0x50-byte vtable. */
extern const s32 D_80151498[20]; /* Complete 0x50-byte vtable. */
void func_80092508(void) {
    MenuCursor *obj = &D_80140100;
    obj->vtable_0C = &D_80151350;
    clear_records(&D_80140100);
    obj->vtable_0C = &D_80151498;
}
