#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Obj800EB09C Obj800EB09C;

extern void *func_800E8A68(Obj800EB09C *obj, u8 slot);

void *func_800EB09C(Obj800EB09C *obj)
{
    return func_800E8A68(obj, 3);
}
