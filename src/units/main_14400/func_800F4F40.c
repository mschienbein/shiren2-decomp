#include "common.h"

typedef unsigned char u8;

/* Partial view of the 0x2C-byte object initialized here. */
typedef struct {
    u8 pad_00[0xA];
    u8 kind_0A;
    u8 pad_0B[0x1F - 0xB];
    u8 kind_1F;
    u8 pad_20[4];
    void *vtable_24;
    u8 pad_28[4];
} Obj800F4F40;

extern unsigned char D_80159680[];

extern void *func_800A38FC(s32 size);
extern void *func_800F4760(void *obj);

/* Factory slot of D_8015CD74 (dispatched by func_800A86EC): kind 9. */
void *func_800F4F40(void) {
    Obj800F4F40 *obj = func_800A38FC(0x2C);

    func_800F4760(obj);
    obj->vtable_24 = D_80159680;
    obj->kind_0A = 9;
    obj->kind_1F = 9;
    return obj;
}
