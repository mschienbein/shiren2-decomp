#include "common.h"
typedef struct { unsigned char pad0[0x95]; unsigned char flags95[32]; } Object;
void func_800EC9D0(Object *object, unsigned char bit) {
    u32 index = bit;
    object->flags95[index >> 3] |= 1 << (index & 7);
}
