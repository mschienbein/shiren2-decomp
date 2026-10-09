#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad00[0x90]; s16 delta90; s16 slot92; s32 (*method94)(void *, s32, s32, u8, s32); } Vtable;
typedef struct { u8 pad00[0x24]; Vtable *vtable24; } Object;

/* D_80158C98 + 0x94 targets func_800E115C. */
s32 func_800E3084(Object *object)
{
    Vtable *table = object->vtable24;
    return table->method94((u8 *)object + table->delta90, 1, 3, 0, 0);
}
