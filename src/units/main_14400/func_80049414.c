#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self, s32 a, s32 b, u8 c, s32 d);
} VtblEntry;

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F[0x5];
    VtblEntry *vtbl_24;
    u8 pad28[0x4A];
    u8 flags_72;
    u8 pad73[0x27];
    u16 flags_9A;
    u8 pad9C[0x68];
    void *field_104; /* retained actor (func_800E2290 returns it; 0x800EBCEC dereferences it) */
} Obj_80049414;

extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
extern s32 func_800E1CD4(Obj_80049414 *obj, s32 kind);
extern s32 func_800E1D14(Obj_80049414 *obj, s32 kind);
extern s32 func_800E04D0(Obj_80049414 *obj);
extern s32 func_800A6FD0(Obj_80049414 *obj);
extern s32 func_801F4F08(Obj_80049414 *obj);
extern s32 func_801F4E2C(Obj_80049414 *obj);

void func_80049414(Obj_80049414 *obj, u32 *flags_out, s32 *mode_out) {
    Obj_80049414 *self = obj;
    VtblEntry *entry;
    u32 flags;
    s32 mode;
    s32 guard;
    s32 special;
    s32 blocked;
    s32 busy;
    u16 bits;
    s32 on;
    s32 set;

    flags = (func_800E1CC4(obj, 1) != 0) << 4;
    if (func_800E1CD4(obj, 0xD)) {
        flags |= 0x20;
    }
    guard = func_800E1CD4(obj, 0xA) || func_800E1CD4(obj, 0xB) || (obj->flags_72 & 1);
    if (guard) {
        flags |= 0x2;
    }
    if (func_800E1CD4(self, 0xE)) {
        flags |= 0x40;
    }
    if (func_800E1D14(self, 0x13)) {
        flags |= 0x1;
    }
    entry = &self->vtbl_24[18];
    special = entry->fn((u8 *)self + entry->delta, 2, 0x11, 0, 0) && !(u8)func_800E04D0(self);
    if (special) {
        flags |= 0x4;
    }
    if (func_800E1CC4(self, 4)) {
        flags |= 0x8;
    }
    on = (obj->flags_1E >> 4) & 1;
    if (on == 1) {
        bits = obj->flags_9A;
        set = bits & 0x40;
        set = set != 0;
        if (set == 1) {
            flags |= 0x80;
        }
        set = bits & 0x200;
        set = set != 0;
        if (set == 1) {
            flags &= ~0x80;
            flags |= 0x800;
        }
    }
    if (func_800E1CC4(self, 2)) {
        flags |= 0x100;
    }
    if (func_800E1CC4(self, 0) && func_800A6FD0(obj) != 1) {
        flags |= 0x200;
    }
    if (func_800E1CC4(self, 5)) {
        flags |= 0x400;
    }
    if (func_801F4F08(obj)) {
        flags |= 0x4000;
    }
    switch ((u8)func_800E04D0(obj)) {
        case 0:
            mode = 4;
            break;
        case 1:
        default:
            mode = 5;
            break;
        case 2:
            mode = 6;
            break;
        case 3:
            mode = 7;
            break;
    }
    if ((func_801F4E2C(obj) ^ 1) != 0) {
        mode = 0;
    }
    if ((obj->flags_1E >> 2) & 1) {
        if (self->flags_72 & 0x10) {
            mode = 3;
        }
    }
    blocked = func_800E1CD4(self, 0xC) || func_800E1CD4(self, 0xD) || func_800E1CD4(self, 0xE);
    if (blocked) {
        mode = 0;
    }
    busy = func_800E1CD4(self, 0xA) || func_800E1CD4(self, 0xB) || (self->flags_72 & 1);
    if (busy) {
        mode = 1;
    }
    if ((obj->flags_1E >> 2) & 1) {
        if (obj->field_104 != 0) {
            mode = 2;
        }
    }
    *flags_out = flags;
    *mode_out = mode;
}
