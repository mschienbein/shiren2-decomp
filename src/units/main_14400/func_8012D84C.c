#include "common.h"
typedef unsigned char u8;
typedef struct { u8 *base; u8 *cur; s32 len; s32 count; } ALHeap;
extern ALHeap D_801CA940;
extern void *func_8002AB40(u8 *file, s32 line, ALHeap *hp, s32 num, s32 size);
void *func_8012D84C(s32 size)
{
    return func_8002AB40(0, 0, &D_801CA940, 1, size);
}
