#include "common.h"

typedef struct {
    unsigned char pad0[0x14];
    void *field_14;
} Obj;

/* CA4E8 stores the failing text pointer at +0x14 (0x800CA544). */
void *func_800CA5EC(Obj *obj, s32 index)
{
    void *result = 0;

    if (index == 0) {
        result = obj->field_14;
    }
    return result;
}
