#include "common.h"

typedef unsigned char u8;

typedef struct Obj800BCB18 Obj800BCB18;

typedef struct {
    u8 pad00[0x9C];
    u8 slot9C;
} Owner800F02B0;

extern s32 func_800AC670(Obj800BCB18 *obj);
extern s32 func_800AFD08(void *table, void *obj);
extern u8 D_80143094[];

static inline s32 item_usable(Obj800BCB18 *item)
{
    return func_800AC670(item) != 1;
}

void func_800F02B0(Owner800F02B0 *self, Obj800BCB18 *item)
{
    if (item != 0 && item_usable(item)) {
        self->slot9C = func_800AFD08(D_80143094, item);
    } else {
        self->slot9C = 0xFF;
    }
}
