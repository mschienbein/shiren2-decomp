#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[4];
    u8 mode04;
    u8 pad05[7];
    s32 kind0C;
} Obj80112CB8;

void func_80112CB8(Obj80112CB8 *obj)
{
    u8 mode = 1;

    if (obj->kind0C == 3) {
        mode = 2;
    }
    obj->mode04 = mode;
}
