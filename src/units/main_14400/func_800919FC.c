#include "common.h"

typedef struct {
    s32 field0;
    s32 field4;
    s32 active;
} Obj800919FC;

s32 func_800919FC(Obj800919FC *obj)
{
    obj->active = 1;
    return -1;
}
