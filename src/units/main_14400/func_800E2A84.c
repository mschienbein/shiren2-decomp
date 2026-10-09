#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Obj800E2A84 Obj800E2A84;
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *obj, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
} VtblEntry800E2A84;
typedef struct {
    u8 pad00[0x90];
    VtblEntry800E2A84 slot90;
} Vtbl800E2A84;
struct Obj800E2A84 {
    u8 pad00[0x24];
    Vtbl800E2A84 *vtbl;
};

s32 func_800E2A84(Obj800E2A84 *self)
{
    return self->vtbl->slot90.fn((u8 *)self + self->vtbl->slot90.delta, 1, 13, 0, 0);
}
