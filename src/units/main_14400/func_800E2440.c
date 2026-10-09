#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0x54];
    u8 flags54;
} Obj800E2440;

void func_800E2440(Obj800E2440 *obj) {
    obj->flags54 &= ~8;
}
