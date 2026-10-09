#include "common.h"

typedef unsigned char u8;

typedef struct Obj Obj;

void *func_800A38FC(s32 size);
Obj *func_800FF480(Obj *obj, u8 level);

/* Monster factory (D_8015CC64 entry): construct in `place`, or in a new 0xA0-byte block. */
Obj *func_800FF440(u8 level, Obj *place)
{
    if (place) {
        return func_800FF480(place, level);
    } else {
        return func_800FF480(func_800A38FC(0xA0), level);
    }
}
