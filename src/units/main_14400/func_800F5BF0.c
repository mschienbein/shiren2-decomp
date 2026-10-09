#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0xA];
    u8 kind_0A;
    u8 padB[0x1F - 0xB];
    u8 kind_1F;
    s32 field_20;
    void *vtable_24;
} Object800F5BF0;

extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object800F5BF0 *);

/* Initialized original vtable; its full type is unresolved. */
extern u8 D_801495A8[];

Object800F5BF0 *func_800F5BF0(void)
{
    Object800F5BF0 *p = func_800A38FC(0x2C);

    func_800F4760(p);
    p->vtable_24 = D_801495A8;
    p->kind_0A = 0x12;
    p->kind_1F = 0x12;
    return p;
}
