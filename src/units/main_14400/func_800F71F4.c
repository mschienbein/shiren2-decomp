#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xA4];
    s32 field_A4;
} Obj;

/* The other entity examined by actor vtable slot +0x40/+0x44
 * (func_800F40F0 reads its bytes +0x1E and +0xA); opaque here. */
typedef struct Entity Entity;

s32 func_800F40F0(Obj *obj, Entity *target, u8 *out);

s32 func_800F71F4(Obj *obj, Entity *target, u8 *out) {
    *out = 0;
    if (target == 0) {
        return 0;
    }
    if (obj->field_A4 == 0) {
        return 0;
    }
    return func_800F40F0(obj, target, out);
}
