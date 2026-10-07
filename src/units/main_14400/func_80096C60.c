#include "common.h"
/* Text-window subobject: func_800487EC reads only its handle at +0xC (prefix view). */
typedef struct { char unknown_0[0xC]; s32 handle_C; } Text80096C60;
typedef struct { short delta; short index; char *(*fn)(void *); } VEntry;
/* Partial view of the window object: vtable at 0x4C, embedded text window at 0x54
 * (0x54..0x63 covers every access its consumer makes). */
typedef struct { char pad[0x4C]; VEntry *vtbl; char unknown_50[4]; Text80096C60 text; } Obj;
void func_800487EC(Text80096C60 *text, s32 row, s32 col, char *str);
void func_80096C60(Obj *self) {
    VEntry *e = &self->vtbl[18];
    func_800487EC(&self->text, 0, 1, e->fn((char *)self + e->delta));
}
