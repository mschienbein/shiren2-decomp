#include "common.h"
typedef struct { unsigned char pad0[0xC]; unsigned char flagsC; } Object;
void func_80116B84(Object *object) { object->flagsC |= 1; }
