#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x7];
    u8 flags_07;
} Obj800CAEF4;

/* Bit masks 1 << n and their complements, n = 0..7. */
extern const u8 D_8015488C[8];
extern const u8 D_80154894[8];

extern void func_800CAD44(Obj800CAEF4 *self);

void func_800CAEF4(Obj800CAEF4 *obj)
{
    u8 flags = obj->flags_07;

    if (flags & D_8015488C[2]) {
        obj->flags_07 = flags & D_80154894[1];
    }
    func_800CAD44(obj);
}
