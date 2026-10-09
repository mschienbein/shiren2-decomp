#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
extern void *func_800A38FC(s32 size);
extern Obj *func_800FA140(Obj *obj, u8 value);
Obj *func_800FA100(u8 value, Obj *storage)
{
    if (storage != 0) {
        return func_800FA140(storage, value);
    }
    return func_800FA140(func_800A38FC(0xC0), value);
}
