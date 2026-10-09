#include "common.h"

typedef unsigned long PiWord;
typedef struct { unsigned char pad00[0xC]; s32 field0C; unsigned char pad10[0xC]; PiWord deviceOffset1C; } Obj;
extern void func_80043BB8(PiWord deviceOffset, PiWord data);

void func_80044228(Obj *obj, PiWord data, PiWord offset)
{
    if (obj->field0C == 0) {
        func_80043BB8(obj->deviceOffset1C + offset, data);
    }
}
