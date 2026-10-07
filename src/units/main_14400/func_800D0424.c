#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    s32 valueC;
} Obj800D0424;

void func_800D0424(Obj800D0424 *obj, s32 value) {
    if (value < obj->valueC) {
        obj->valueC = value;
    }
}
