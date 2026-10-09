#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct { s16 delta; s16 index; void *(*func)(void *, u32); } VtblEntry;
typedef struct { u8 pad0[0x38]; VtblEntry entry38; } Vtbl;
typedef struct { s32 unk0; Vtbl *vtbl; } Base;
typedef struct { u8 pad0[0xC]; Base base; } Obj;

void *func_8011546C(Obj *obj, u32 index) {
    Base *base = &obj->base;
    return base->vtbl->entry38.func((u8 *)base + base->vtbl->entry38.delta, index);
}
