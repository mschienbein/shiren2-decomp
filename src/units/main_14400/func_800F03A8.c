#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field_00; } S;
typedef struct { u8 pad_00[0x9A]; u16 field_9A; } T;
typedef T Object;
extern void *D_801476B8;
extern void *func_800A65E4(S *p, T *q, void *target);
extern void func_800A6690(Object *, unsigned char *, s32);

void func_800F03A8(T *object) {
    S direction;
    object->field_9A |= 0x80;
    func_800A65E4(&direction, object, D_801476B8);
    func_800A6690(object, &direction.field_00, 1);
}
