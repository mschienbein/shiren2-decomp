#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xA];
    u8 xA;
    u8 padB[0x1F - 0xB];
    u8 x1F;
    u8 pad20[0x24 - 0x20];
    void *vtable;
    u8 pad28[0x2C - 0x28];
} Obj800F4E00;

extern u8 D_80149268[];

void *func_800A38FC(s32 size);
void *func_800F4760(Obj800F4E00 *obj);

/* Factory (D_8015CD74 entry): allocate and construct a 0x2C-byte object. */
Obj800F4E00 *func_800F4E00(void)
{
    Obj800F4E00 *obj = func_800A38FC(sizeof(Obj800F4E00));

    func_800F4760(obj);
    obj->vtable = D_80149268;
    obj->xA = 5;
    obj->x1F = 5;
    return obj;
}
