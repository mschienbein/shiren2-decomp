#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    void *(*func)(void *self);
} VtableEntry;

typedef struct {
    char pad0[0x98];
    VtableEntry entry98;
} Vtable800EA254;

typedef struct {
    s32 bits;
    s32 unk4;
} Flags800EA254;

typedef struct {
    char pad0[0x20];
    s32 flags;
    Vtable800EA254 *vtable;
    char pad28[0x80 - 0x28];
    s32 unk80;
} Obj800EA254;

extern s32 D_80148270;
extern void func_80136908(Flags800EA254 *flags);
extern void *func_800E8A68(Obj800EA254 *obj, u8 slot);
extern void func_8010ED94(void *item, Flags800EA254 *flags);
extern void func_8010CDCC(void *item, Flags800EA254 *flags);
extern void func_800E8790(Obj800EA254 *obj, Flags800EA254 *flags);
extern void *func_800CDA70(void *item, u8 kind);
extern s32 func_800A99D0(void);
extern s32 func_800E1CD4(Obj800EA254 *obj, s32 kind);
extern s32 func_800E1CC4(Obj800EA254 *obj, s32 kind);

void func_800EA254(Obj800EA254 *obj) {
    Flags800EA254 flags;
    Flags800EA254 *fp = &flags;
    void *item;
    s32 extra;

    func_80136908(fp);
    extra = obj->unk80;
    flags.bits |= extra;
    item = func_800E8A68(obj, 3);
    if (item != 0) {
        func_8010ED94(item, fp);
    }
    item = func_800E8A68(obj, 4);
    if (item != 0) {
        func_8010CDCC(item, fp);
    }
    func_800E8790(obj, fp);
    item = obj->vtable->entry98.func((char *)obj + obj->vtable->entry98.delta);
    if (item != 0 && func_800CDA70(item, 0x9C) != 0) {
        flags.bits |= 0x10;
    }
    if (func_800A99D0() != 0) {
        flags.bits &= ~0x01000000;
    }
    if (func_800E1CD4(obj, 15) != 0) {
        flags.bits &= ~D_80148270;
    }
    if (func_800E1CC4(obj, 2) != 0) {
        flags.bits &= ~0x02000000;
    }
    obj->flags = flags.bits;
}
