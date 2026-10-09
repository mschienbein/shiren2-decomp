#include "common.h"

typedef struct VTable VTable;
typedef struct {
    unsigned char pad00[0xA];
    unsigned char type0A;
    unsigned char pad0B[0x14];
    unsigned char type1F;
    unsigned char pad20[4];
    VTable *vtable24;
    s32 field28;
} Object;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *obj);
extern VTable D_80149200;

Object *func_800F4DB0(void)
{
    Object *obj = func_800A38FC(0x2C);
    func_800F4760(obj);
    obj->vtable24 = &D_80149200;
    obj->type0A = 4;
    obj->type1F = 4;
    return obj;
}
