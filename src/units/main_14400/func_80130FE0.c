#include "common.h"

typedef struct { s32 unknown00[2]; s32 field08; } Object;
typedef struct { Object *object; s32 value; } Arguments;
extern s32 func_80131F04(s32, Arguments *);
void func_80130FE0(Object *object, s32 value) {
    Arguments arguments;
    arguments.value = value;
    arguments.object = object;
    object->field08 = func_80131F04(0x206, &arguments);
}
