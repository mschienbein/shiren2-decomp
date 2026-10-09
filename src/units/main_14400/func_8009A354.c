#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field0; u8 field1; } Object;
s32 func_8009A354(void *object) { return ((Object *)object)->field1 == 0xEE; }
