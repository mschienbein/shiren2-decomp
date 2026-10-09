#include "common.h"

typedef struct {
    unsigned char pad0[0x54];
    unsigned char flags54;
} Obj800E23F0;

s32 func_800E23F0(Obj800E23F0 *obj)
{
    if (obj->flags54 & 4) {
        return 1;
    }
    return 0;
}
