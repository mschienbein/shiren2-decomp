#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0xC]; u8 field_0C; } Object;
void func_80116B74(Object *object) { object->field_0C &= ~1; }
