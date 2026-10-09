#include "common.h"
typedef struct { unsigned char pad0[0xD]; unsigned char flagsD; } Object;
s32 func_80128EDC(Object *object) { return object->flagsD >> 7; }
