#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x18];
    s16 delta_18;
    s16 pad1A;
    void (*func_1C)(void *self, s32 size, void *data);
} VTable80112DE4;

typedef struct {
    u8 pad0[0x18];
    VTable80112DE4 *vtable_18;
} Obj80112DE4;

extern u8 D_8015D71C[];
extern u8 D_80148644[];

void func_800CA4A4(void *, void *);

void func_80112DE4(Obj80112DE4 *obj)
{
    func_800CA4A4(obj, D_8015D71C);
    obj->vtable_18->func_1C((u8 *)obj + obj->vtable_18->delta_18, 4, D_80148644);
}
