#include "common.h"

typedef struct Obj800BCB18 Obj800BCB18;

typedef struct {
    void *container;
    Obj800BCB18 *element;
} Ref800D0248;

s32 func_800AC670(Obj800BCB18 *obj);
s32 func_800CD090(void *container, void *element);

s32 func_800D0248(Ref800D0248 *ref)
{
    if (ref->container == 0 || ref->element == 0) {
        return 0;
    }
    if (func_800AC670(ref->element) == 0 && func_800CD090(ref->container, ref->element) >= 0) {
        return 1;
    }
    ref->element = 0;
    return 0;
}
