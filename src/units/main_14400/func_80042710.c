#include "common.h"
typedef struct MenuCursor { s32 index; void *records; s32 unknown_08; void *vtable_0C; } MenuCursor;
extern MenuCursor D_80140100;
extern s32 func_80091B30(MenuCursor *cursor);
s32 func_80042710(void) { return func_80091B30(&D_80140100); }
