#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0xB0];
    short adjustB0;
    short padB2;
    /* D_8015B440 slot 0xB4 targets func_80100F24, which returns 1. */
    s32 (*fieldB4)(void *, void *);
} VTable;
typedef struct { u8 pad0[0x24]; VTable *field24; } Object;
extern void *func_800A6CF0(Object *);

s32 func_80100EE8(Object *self)
{
    void *target = func_800A6CF0(self);
    return self->field24->fieldB4((u8 *)self + self->field24->adjustB0, target);
}
