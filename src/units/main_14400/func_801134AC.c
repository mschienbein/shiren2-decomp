#include "common.h"

typedef struct {
    unsigned char pad0[0xC];
    unsigned char flagsC;
} Obj801134AC;

void func_801134AC(Obj801134AC *obj)
{
    obj->flagsC &= ~1;
}
