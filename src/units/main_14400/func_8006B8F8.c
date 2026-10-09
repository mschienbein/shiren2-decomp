#include "common.h"

typedef struct { u32 words[2]; } Gfx;
typedef struct { Gfx *commands; s32 count; } DisplayList;
extern Gfx D_8013CB38[];

void func_8006B8F8(DisplayList *list)
{
    list->commands = D_8013CB38;
    list->count = 1;
}
