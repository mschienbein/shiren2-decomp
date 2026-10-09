#include "common.h"

typedef struct {
    unsigned char pad_00[0x90];
    signed short delta_90;
    unsigned short reserved_92;
    s32 (*action_94)(void *, s32, s32, unsigned char, s32);
} ActionVtable;
typedef struct { unsigned char pad_00[0x24]; ActionVtable *vtable_24; } Object;

/* D_80158C98 slot 18 targets 800E115C, including its integer result. */
s32 func_800E3260(Object *object)
{
    ActionVtable *vtable = object->vtable_24;
    return vtable->action_94((char *)object + vtable->delta_90, 0, 0, 0xFE, 0);
}
