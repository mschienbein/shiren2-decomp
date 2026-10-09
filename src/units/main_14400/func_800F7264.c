#include "common.h"

typedef unsigned char u8;

typedef struct Obj Obj;

/* Embedded 0x18-byte collection; its layout belongs to func_800CEC90. */
typedef struct { u8 storage[0x18]; } Collection800F7264;

typedef struct {
    u8 pad0[0x8C];
    Collection800F7264 list8C; /* embedded list object */
} Self800F7264;

void func_800CD468(void *list);
void func_800E032C(Self800F7264 *, Obj *);
void func_800CE9C4(void *, Obj *);

void func_800F7264(Self800F7264 *self, Obj *obj)
{
    Collection800F7264 *list = &self->list8C;

    func_800CD468(list);
    func_800E032C(self, obj);
    func_800CE9C4(list, obj);
}
