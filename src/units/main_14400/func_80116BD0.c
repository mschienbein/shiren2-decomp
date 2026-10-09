#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0xC]; u8 field0C; } FlagView;

void func_80116BD0(FlagView *obj)
{
    obj->field0C &= 0xF7;
}
