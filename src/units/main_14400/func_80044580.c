#include "common.h"

typedef struct {
    char pad0[0x1C];
    u32 romBase; /* cartridge ROM address of the table */
} Object;

void func_8006AAF0(void *dst, u32 devAddr, s32 size);

/* Read one 32-bit word at `offset` from the object's ROM table. */
u32 func_80044580(Object *obj, s32 offset)
{
    u32 word;

    func_8006AAF0(&word, obj->romBase + offset, 4);
    return word;
}
