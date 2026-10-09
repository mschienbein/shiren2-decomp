#include "common.h"

typedef struct {
    short delta;
    short index;
    s32 (*fn)(void *self);
} VEntry;

typedef struct {
    unsigned char pad0[0x10];
    VEntry slot2;
} VTable;

typedef struct {
    void *pool_0;
    VTable *vtbl;
} Obj;

/* Slot +0x14 targets the signed count getters; this wrapper discards the result. */
void func_800CE58C(Obj *obj)
{
    obj->vtbl->slot2.fn((char *)obj + obj->vtbl->slot2.delta);
}
