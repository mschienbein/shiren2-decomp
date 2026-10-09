#include "common.h"

typedef unsigned char u8;

/* Partial view: only the level byte at 0x33 is read. */
typedef struct Obj800F6038 {
    u8 pad_00[0x33];
    u8 level_33;
} Obj800F6038;

extern u32 D_8013960C;

s32 func_800E1CC4(void *obj, s32 kind);
void func_800E03BC(void *self, s32 arg1);

void func_800F6038(Obj800F6038 *self, s32 arg)
{
    u8 level = arg;

    if (self->level_33 == level) {
        return;
    }
    if (self->level_33 < level && func_800E1CC4(self, 2) != 0) {
        return;
    }
    D_8013960C <<= 1;
    func_800E03BC(self, level);
    D_8013960C >>= 1;
}
