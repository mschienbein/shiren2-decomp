#include "common.h"

typedef unsigned char u8;

/* Partial view: only the flag byte at 0x0D is touched. */
typedef struct Obj80113FC8 {
    u8 pad_00[0xD];
    u8 flags_0D;
} Obj80113FC8;

void func_80113FC8(Obj80113FC8 *obj)
{
    obj->flags_0D |= 4;
}
