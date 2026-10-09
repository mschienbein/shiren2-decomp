#include "common.h"

typedef unsigned char u8;
typedef struct ObjectCountView {
    u8 pad_00[0x6C];
    s32 count_6C;
} ObjectCountView;

void func_800E32BC(ObjectCountView *object, s32 increment)
{
    object->count_6C += increment;
}
