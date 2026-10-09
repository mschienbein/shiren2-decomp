#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x4];
    s16 state_04;
    u8 pad6[0x14 - 0x6];
    s32 handle_14;
} Obj80088204;

extern void func_8007935C(s32 i);

void func_80088204(Obj80088204 *obj)
{
    if (obj->handle_14 >= 0) {
        func_8007935C(obj->handle_14);
    }
    obj->state_04 = 4;
}
