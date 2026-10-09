#include "common.h"

typedef struct { s32 x; s32 y; } Position;
typedef struct { Position min; Position max; } Rectangle;
/* Returns its rectangle by value through the hidden result pointer. */
extern Rectangle func_800B3024(void *object);
extern void func_800B2DEC(Rectangle *rect);

void func_800B2DC4(void *object)
{
    Rectangle rect = func_800B3024(object);
    func_800B2DEC(&rect);
}
