#include "common.h"
typedef struct { unsigned char pad_00[0xC]; unsigned char flags_0C; } Flags80128B14;
void func_80128B14(Flags80128B14 *object)
{
    object->flags_0C |= 8;
}
