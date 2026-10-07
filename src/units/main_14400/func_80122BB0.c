#include "common.h"
typedef struct { short offset; short pad; s32 (*func)(void *self); } VtEntry80122BB0;
typedef struct { unsigned char pad0[0x20]; VtEntry80122BB0 entry_20; } Vtable80122BB0;
typedef struct { s32 field_0; Vtable80122BB0 *vtable; } Sub80122BB0;
typedef struct { unsigned char pad0[0xC]; Sub80122BB0 sub; unsigned char pad14[0x14]; unsigned char field_28; } Obj80122BB0;
s32 func_80122BB0(Obj80122BB0 *obj, s32 kind) {
    s32 result;
    if (kind == 0x1D) {
        return obj->field_28 != 0;
    }
    if (kind == 0x12) {
        Sub80122BB0 *sub = &obj->sub;
        return sub->vtable->entry_20.func((char *)sub + sub->vtable->entry_20.offset) != 0;
    }
    result = 0;
    if (kind == 0x10 || kind == 0x11 || kind == 0xB || kind == 0x16) result = 1;
    return result;
}
