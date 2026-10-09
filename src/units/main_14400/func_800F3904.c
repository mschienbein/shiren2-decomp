#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x8C]; void *field8C; } Owner;
extern s32 func_800CD5C0(void *container, void *item);

s32 func_800F3904(Owner *owner, void *item)
{
    return func_800CD5C0(owner->field8C, item);
}
