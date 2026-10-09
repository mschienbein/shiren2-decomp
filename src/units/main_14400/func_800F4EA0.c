#include "common.h"

typedef unsigned char u8;

/* Partial view of the 0x2C-byte object initialized here. */
typedef struct Obj800F4EA0 {
    u8 pad_00[0xA];
    u8 kind_0A;
    u8 pad_0B[0x1F - 0xB];
    u8 kind_1F;
    u8 pad_20[4];
    void *vtable_24;
    u8 pad_28[4];
} Obj800F4EA0;

/* Initialized original vtable, not BSS. Its full type is unresolved. */
extern unsigned char D_80149338[];

void *func_800A38FC(s32 size);
void *func_800F4760(void *obj);

/* Factory slot of D_8015CD74 (dispatched by func_800A86EC). */
void *func_800F4EA0(void)
{
    Obj800F4EA0 *obj = func_800A38FC(0x2C);

    func_800F4760(obj);
    obj->vtable_24 = D_80149338;
    obj->kind_0A = 7;
    obj->kind_1F = 7;
    return obj;
}
