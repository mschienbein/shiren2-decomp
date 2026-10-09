#include "common.h"

typedef struct { s32 x; s32 y; } Vec2;
typedef struct {
    unsigned char pad_00[0x52];
    unsigned char mode_52;
    unsigned char mode_53;
    unsigned char pad_54[0x10];
    Vec2 target_64;
} MovingObject;

void func_800E5F5C(MovingObject *object, Vec2 *target)
{
    object->target_64 = *target;
    object->mode_52 = 2;
    object->mode_53 = 2;
}
