#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x58]; void *field58; } Object;
extern s32 func_800E8694(void *object);
extern s32 func_800E8350(void *object);
extern void *func_800A492C(void *object, s32 mode, s32 flag1, s32 flag2);
extern s32 func_800E66EC(void *object);
extern s32 func_800E7104(void *object);

s32 func_800F442C(Object *object)
{
    void *target;
    if (func_800E8694(object)) {
        return func_800E8350(object);
    } else {
        target = func_800A492C(object, 2, 1, 1);
        object->field58 = target;
        if (target != 0)
            return func_800E7104(object);
        else
            return func_800E66EC(object);
    }
}
