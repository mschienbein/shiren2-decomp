#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[0xE4]; u16 flags_E4; } Flags800EE094;
s32 func_800EE094(Flags800EE094 *object)
{
    return (object->flags_E4 >> 8) & 1;
}
