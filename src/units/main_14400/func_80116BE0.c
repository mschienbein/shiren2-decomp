#include "common.h"

typedef unsigned char u8;
typedef struct ObjectFlagsView {
    u8 pad_00[0xC];
    u8 flags_0C;
} ObjectFlagsView;

void func_80116BE0(ObjectFlagsView *object)
{
    object->flags_0C |= 8;
}
