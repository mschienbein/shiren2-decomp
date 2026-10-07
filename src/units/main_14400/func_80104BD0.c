#include "common.h"

typedef unsigned char u8;

/* Target entity (passed as the target of func_800A6420); opaque here. */
typedef struct Object Object;

/* Class D_8015BB38 instance: target-entity pointer at +0xA0. */
typedef struct {
    u8 pad0[0xA0];
    Object *target;
} Obj;

Object *func_80104BD0(Obj *obj) {
    return obj->target;
}
