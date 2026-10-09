#include "common.h"
extern char D_80151350[];
typedef struct MenuCursor { s32 index; void *records; s32 unknown_08; void *vtable_0C; } MenuCursor;
extern MenuCursor D_801400F0;
void func_800923A0(void) { s32 unused[4]; D_801400F0.vtable_0C = D_80151350; }
