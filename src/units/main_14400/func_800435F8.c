#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct { u8 pad00[8]; s16 delta08; s16 pad0A; void (*run0C)(void *); } VTable;
/* Complete 0x44-byte table, compatible with the view in func_800AA9FC. */
typedef struct { VTable prefix; u8 remaining[0x34]; } WholeVTable;
extern u8 D_80153B40[];
extern const WholeVTable D_8014A7A8;
typedef struct {
    u8 kind;
    u8 pad1[3];
    s32 unk4;
    const void *vtable;
} Obj;
void func_800C2440(Obj *, u8, s8, s8);

Obj *func_800435F8(Obj *obj, u8 kind, s8 a, s8 b)
{
    obj->vtable = D_80153B40;
    obj->kind = kind;
    obj->unk4 = 0;
    obj->vtable = &D_8014A7A8.prefix;
    func_800C2440(obj, kind, a, b);
    return obj;
}
