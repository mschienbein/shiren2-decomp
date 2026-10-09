#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x88]; u8 field88; } Object;
extern s32 func_800F0F84(Object *object, u8 index);
s32 func_800F1024(Object *object) { return func_800F0F84(object, object->field88); }
