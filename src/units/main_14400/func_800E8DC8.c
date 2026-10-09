#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u8 pad0[4]; u8 field4; } Target;
typedef struct {
    u8 pad0[0x68];
    s16 adjust68; s16 pad6A;
    u32 (*field6C)(void *);
    u8 pad70[0x30];
    s16 adjustA0; s16 padA2;
    /* Concrete player D_80159008 uses func_800EE060 at slot 0xA4. */
    Target *(*fieldA4)(void *, u8);
} VTable;
typedef struct { u8 pad0[0x24]; VTable *field24; } Object;
extern s32 func_800E0F40(Object *obj);

s32 func_800E8DC8(Object *self, s16 bonus)
{
    s32 value = (u16)self->field24->field6C((u8 *)self + self->field24->adjust68) + bonus / 2 + 8;
    Target *target;
    if (value < 0) {
        value = 0;
    }
    target = self->field24->fieldA4((u8 *)self + self->field24->adjustA0,
                                   (u8)func_800E0F40(self));
    return (value * target->field4) >> 4;
}
