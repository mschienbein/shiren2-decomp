#include "common.h"

typedef struct Obj Obj;
extern void *func_800A38FC(s32 size);
extern Obj *func_800FF020(Obj *obj, unsigned char value);

Obj *func_800FEFE0(unsigned char value, Obj *obj)
{
    if (obj != 0) {
        return func_800FF020(obj, value);
    } else {
        return func_800FF020(func_800A38FC(0xA0), value);
    }
}
