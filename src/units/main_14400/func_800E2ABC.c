#include "common.h"

typedef struct {
    unsigned char pad00[0x90];
    short adjust90;
    short pad92;
    s32 (*method94)(void *, s32, s32, unsigned char, s32);
} VTable;
typedef struct { unsigned char pad00[0x24]; VTable *vtable24; } Obj;

s32 func_800E2ABC(Obj *obj)
{
    VTable *table = obj->vtable24;
    return table->method94((char *)obj + table->adjust90, 0, 13, 255, 0);
}
