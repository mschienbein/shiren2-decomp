#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x72]; u8 field72; } FlagView;

void func_800E2684(FlagView *obj, s32 mask)
{
    obj->field72 |= mask;
}
