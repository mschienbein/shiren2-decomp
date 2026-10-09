#include "common.h"

typedef unsigned char u8;

typedef struct { u32 a; u32 b; } Pair;
typedef struct { u8 value; } Dir;
typedef struct {
    Pair pos;            /* 0x00 */
    u8 pad08[0x52 - 0x08];
    u8 field_52;         /* 0x52 */
    u8 field_53;         /* 0x53 */
    u8 pad54[0x64 - 0x54];
    Pair target;         /* 0x64 */
} Obj;

void *func_800A6538(void *out_direction, void *obj, void *target);
void func_800A4EC0(void *state, void *value);
void func_800A4F58(void *a, Dir *b);
s32 func_800A251C(Pair *x, Pair *y);

static inline s32 settle(Obj *obj, Pair *target, s32 done) {
    if (func_800A251C(target, &obj->pos) != 0) {
        obj->field_52 = done;
        obj->target.a = 0;
        target->b = 0;
    }
    return 1;
}

s32 func_800E53A8(Obj *obj) {
    Dir a;
    Dir b;
    s32 done = 1;

    if (obj->field_52 == done) {
        return 0;
    }
    switch (obj->field_53) {
    case 1: {
        Pair *target = &obj->target;
        func_800A6538(&a, obj, target);
        func_800A4EC0(obj, &a);
        return settle(obj, target, done);
    }
    case 2: {
        Pair *target = &obj->target;
        func_800A6538(&b, obj, target);
        func_800A4F58(obj, &b);
        return settle(obj, target, done);
    }
    }
    return 0;
}
