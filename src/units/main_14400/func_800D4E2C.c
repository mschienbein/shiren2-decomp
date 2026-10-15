#include "common.h"
#include "map_rect.h"
typedef struct Object Object;
extern MapRect D_80147F80;
extern Object D_80147F90;
extern Object *func_800D4C44(Object *object);
/* Startup callbacks have no result, even when they call a constructor. */
void func_800D4E2C(void)
{
    MapRect rectangle;
    rectangle.min.x = 13;
    rectangle.min.y = 10;
    rectangle.max.x = 17;
    rectangle.max.y = 16;
    D_80147F80.min = rectangle.min;
    D_80147F80.max = rectangle.max;
    func_800D4C44(&D_80147F90);
}
