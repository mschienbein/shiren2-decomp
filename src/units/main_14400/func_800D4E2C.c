#include "common.h"
typedef struct { s32 first; s32 second; } Pair;
typedef struct { Pair field_0; Pair field_8; } Rectangle;
typedef struct Object Object;
extern Rectangle D_80147F80;
extern Object D_80147F90;
extern Object *func_800D4C44(Object *object);
/* Startup callbacks have no result, even when they call a constructor. */
void func_800D4E2C(void)
{
    Rectangle rectangle;
    rectangle.field_0.first = 13;
    rectangle.field_0.second = 10;
    rectangle.field_8.first = 17;
    rectangle.field_8.second = 16;
    D_80147F80.field_0 = rectangle.field_0;
    D_80147F80.field_8 = rectangle.field_8;
    func_800D4C44(&D_80147F90);
}
