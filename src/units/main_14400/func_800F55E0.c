#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xA];
    u8 field_0A;
    u8 pad0B[0x1F - 0xB];
    u8 field_1F;
    u8 pad20[0x24 - 0x20];
    void *vtable_24;
    u8 pad28[0x2C - 0x28];
} Object;

/* Initialized original vtable; its full type is unresolved. */
extern unsigned char D_80149470[];

void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *);

/* Factory slot of D_8015CD74 (no arguments). */
Object *func_800F55E0(void) {
    Object *obj = func_800A38FC(sizeof(Object));

    func_800F4760(obj);
    obj->vtable_24 = D_80149470;
    obj->field_0A = 0xD;
    obj->field_1F = 0xD;
    return obj;
}
