#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef struct {
    u32 pad : 18;
    u32 bit13 : 1;
    u32 bit12 : 1;
    u32 rest : 12;
} Flags;
typedef struct Obj Obj;
typedef struct {
    s16 offset;
    u8 pad2[2];
    s32 (*func)(void *self, s32 arg);
} VtEntry;
typedef struct {
    u8 pad0[0x50];
    VtEntry entry_50;
} Vtable;
struct Obj {
    u8 pad0[0x1C];
    u16 field_1C;
    u8 pad1E[2];
    Flags flags;
    Vtable *vtable;
};
extern u8 D_80147620[];
s32 func_800C587C(void *a, u8 b);
static inline s32 Flags_bit13(Flags *flags) { return flags->bit13; }
static inline s32 Flags_bit12(Flags *flags) { return flags->bit12; }

s32 func_800A5388(Obj *obj, s32 input_mask) {
    Flags fl;
    u16 mask = input_mask;

    if (obj->field_1C & 1) {
        return 1;
    }
    fl = obj->flags;
    if (Flags_bit13(&fl) && !(mask & 0x10)) {
        return 2;
    }
    if (mask & 1) {
        return 0;
    }
    if (Flags_bit12(&fl) || (mask & 2)) {
        return 1;
    }
    if (!(mask & 4)) {
        return func_800C587C(D_80147620, obj->vtable->entry_50.func((u8 *)obj + obj->vtable->entry_50.offset, 1)) != 0;
    }
    return 0;
}
