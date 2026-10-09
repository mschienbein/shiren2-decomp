#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
/* Slot 0x94: func_800E115C and the derived func_800F212C. */
typedef struct {
    u8 pad0[0x90]; s16 adjust90; s16 pad92;
    s32 (*method94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct { u8 pad0[0x24]; const VTable *vtable24; } Object;
s32 func_800E2CEC(Object *object) {
    return object->vtable24->method94((u8 *)object + object->vtable24->adjust90, 1, 9, 0, 0);
}
