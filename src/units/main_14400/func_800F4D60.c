#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0xA];
    u8 field0A;
    u8 pad0B[0x1F - 0xB];
    u8 field1F;
    u8 pad20[4];
    void **vtable24;
    u8 pad28[4];
} Object;

extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *);
extern void *D_80149198[];

/* Zero-argument factory-table entry (D_8015CD74). */
Object *func_800F4D60(void)
{
    Object *obj = func_800A38FC(sizeof(Object));

    func_800F4760(obj);
    obj->vtable24 = D_80149198;
    obj->field0A = 3;
    obj->field1F = 3;
    return obj;
}
