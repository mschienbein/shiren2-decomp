#include "common.h"
typedef unsigned char u8;
/* The +0x94 status-action slot includes func_800F426C. */
typedef struct { u8 pad0[0x90]; short delta90; short pad92; s32 (*method94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vtable; } Object;
s32 func_800E3004(Object *object) {
    return object->vtable->method94((char *)object + object->vtable->delta90, 0, 4, 0xFE, 0);
}
