#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x1E]; u8 field1E; } FlagView;

s32 func_800A7D60(FlagView *obj)
{
    return (obj->field1E & 0xC) != 0;
}
