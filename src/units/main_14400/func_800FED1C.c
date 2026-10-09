#include "common.h"

typedef struct { unsigned char unknown00[0x89]; unsigned char field89; } Object;
s32 func_800FED1C(Object *object) { return -(s32)object->field89; }
