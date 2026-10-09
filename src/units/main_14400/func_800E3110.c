#include "common.h"
typedef unsigned char u8;
/* Slot 94: func_800EA38C / func_800F212C. */
typedef struct { u8 pad_00[0x90]; short adjust_90; short reserved_92; s32 (*call_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_00[0x24]; VTable *vtable_24; } Obj;
s32 func_800E3110(Obj *obj) {
    VTable *vtable = obj->vtable_24;
    return vtable->call_94((u8 *)obj + vtable->adjust_90, 1, 2, 0, 0);
}
