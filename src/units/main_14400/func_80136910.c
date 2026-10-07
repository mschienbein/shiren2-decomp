#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    void *field_0;
    u32 field_4;
    u32 field_8;
    u16 field_C;
    u16 field_E;
    u8 field_10;
} Obj80136910;

void func_80136910(Obj80136910 *obj, void *a, u32 c, u32 b, u32 e)
{
    obj->field_0 = a;
    obj->field_4 = b;
    obj->field_8 = 0;
    obj->field_C = c;
    obj->field_10 = 10;
    obj->field_E = e;
}
