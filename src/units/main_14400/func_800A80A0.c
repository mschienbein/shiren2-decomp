#include "common.h"
typedef struct { unsigned char pad_00[0x20]; u32 flags_20; } Flags800A80A0;
void func_800A80A0(Flags800A80A0 *object, u32 mask)
{
    object->flags_20 |= mask;
}
