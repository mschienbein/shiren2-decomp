#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 unk0; s32 value; } Base800EEB50;
typedef struct { u8 pad0[0x98]; s16 delta; s16 pad9A; void *(*fn)(void *); } Vtbl800EEB50;
typedef struct { u8 pad0[0x24]; Vtbl800EEB50 *vtable; u8 pad28[0x5C]; Base800EEB50 base; } Obj800EEB50;
typedef struct { s32 unk0; void *unk4; s32 unk8; s32 unkC; } Iter800EEB50;
Iter800EEB50 *func_800CEB20(Iter800EEB50 *it, void *list);
s32 func_800CEBA0(Iter800EEB50 *it);
void *func_800CEC68(Iter800EEB50 *it);
void func_800ACE34(void *item);
void func_800EEB50(Obj800EEB50 *obj, s32 value) {
    Base800EEB50 *base = obj != 0 ? &obj->base : 0;
    Vtbl800EEB50 *vt;
    void *list;
    Iter800EEB50 it;
    base->value = value;
    vt = obj->vtable;
    list = vt->fn((u8 *)obj + vt->delta);
    if (list != 0) {
        func_800CEB20(&it, list);
        while (func_800CEBA0(&it)) {
            func_800ACE34(func_800CEC68(&it));
        }
    }
}
