#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[8]; u16 count; u16 limit; } Iterator;
s32 func_800C559C(Iterator *iterator)
{
    return iterator->count <= iterator->limit;
}
