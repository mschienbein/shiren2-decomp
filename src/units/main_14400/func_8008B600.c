#include "common.h"
typedef struct { unsigned char pad0[4]; short state4; unsigned char pad6[0x56]; s32 y5C; unsigned char pad60[8]; s32 x68; } Object;
extern void func_800621C4(s32 y0, s32 x0, s32 y1, s32 x1);
void func_8008B600(Object *object) {
    func_800621C4(object->y5C, object->x68, object->y5C, object->x68);
    object->state4 = 4;
}
