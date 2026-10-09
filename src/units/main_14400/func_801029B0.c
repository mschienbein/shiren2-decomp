#include "common.h"

typedef unsigned char u8;
typedef struct { u8 fields[0x1F]; } Object;
extern void *func_800B4928(void *position);
extern void *func_800B4D80(void *position);
extern s32 func_80102A78(void *object);
extern s32 func_800A692C(void *object, s32 kind);

s32 func_801029B0(void *object, void *position)
{
    Object *first = func_800B4928(position);
    Object *target;
    s32 result;
    if (first != 0 && ((first->fields[0x1E] >> 1) & 1))
        return 1;
    target = func_800B4D80(position);
    result = 0;
    if (target != 0) {
        if (target->fields[0] == 0x10 &&
            (!(target->fields[2] & 0x10) || func_80102A78(object)) &&
            !((target->fields[0xC] >> 1) & 1)) {
            if (func_800A692C(object, 0x15) == 0)
                result = 1;
        }
    }
    return result;
}
