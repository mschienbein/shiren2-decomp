#include "common.h"

typedef unsigned char u8;

/* Partial view: only the flag byte at 0x54 is touched. */
typedef struct Obj800E2420 {
    u8 pad_00[0x54];
    u8 flags_54;
} Obj800E2420;

void func_800E2420(Obj800E2420 *obj)
{
    obj->flags_54 |= 2;
}
