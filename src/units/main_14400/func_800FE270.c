#include "common.h"
typedef unsigned char u8;
typedef struct Object Object;
extern void *func_800A38FC(s32 size);
extern Object *func_800FE2B0(Object *object, u8 value);
Object *func_800FE270(u8 value, Object *storage)
{
    Object *result;
    if (storage == 0) {
        result = func_800FE2B0(func_800A38FC(0xA0), value);
    } else {
        result = func_800FE2B0(storage, value);
    }
    return result;
}
