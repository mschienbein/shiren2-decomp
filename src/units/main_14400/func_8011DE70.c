#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x11]; u8 field11; } Object;
s32 func_8011DE70(Object *object) { return object->field11 != 0; }
