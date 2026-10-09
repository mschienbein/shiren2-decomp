#include "common.h"

typedef unsigned char u8;
typedef struct ObjectFlagView {
    u8 pad_00[0x1E];
    u8 flags_1E;
} ObjectFlagView;

s32 func_800A7D70(ObjectFlagView *object)
{
    return (object->flags_1E >> 3) & 1;
}
