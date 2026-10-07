#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x4C];
    void *vtable;
    u8 pad50[0xAC];
    s32 count;
    u8 pad100[0xC];
    s32 x10C;
    void *x110;
    u8 pad114[0x2C];
} Loader;
typedef struct {
    u8 pad0[0xC0];
    void *xC0;
    void *xC4;
    s32 xC8;
    void *xCC;
    s32 xD0;
} Unit;
typedef struct { s32 value; s32 x4; } ReadResult;
extern u8 D_80153078[];
extern u8 D_80151EC8[];
extern u8 D_80151E38[];
void *func_800953C0(void *o);
void func_8009F680(Loader *loader, s32 arg);
s32 func_800CD278(void *container);
s32 func_800AC220(void);
void func_8009FAF0(Loader *loader, u32 value);
void func_8009FBB0(Loader *loader);
s32 func_800957C0(void *obj, void *out, s32 a2, void *a3, s32 a4);
s32 func_8009FB00(Loader *loader);
s32 func_8009FB28(Loader *loader, s32 index);
s32 func_8004505C(s32 id, void *outKind, void *outLevel);
void *func_80121788(void);
void *func_800D7A24(u8 x, u8 y);
void *func_8011422C(void *obj);
s32 func_800CD538(void *container, void *item);
s32 func_800D86F4(Unit *unit) {
    Loader loader;
    ReadResult result;
    u8 x;
    u8 y;
    Loader *p = &loader;
    s32 failed;
    s32 count;
    s32 i;
    unit->xD0 = 0;
    func_800953C0(p);
    p->vtable = D_80153078;
    loader.x10C = -1;
    loader.x110 = D_80151EC8;
    func_8009F680(p, 0);
    if (p->count <= 0) {
        p->vtable = D_80151E38;
        return -3;
    }
    if (func_800CD278(unit->xCC) <= 0) {
        p->vtable = D_80151E38;
        return -4;
    }
    failed = func_800AC220() != 1;
    if (failed) {
        p->vtable = D_80151E38;
        return -5;
    }
    func_8009FAF0(p, 1);
    func_8009FBB0(p);
    failed = func_800957C0(p, &result, 1, 0, 0) != 1;
    if (failed) {
        p->vtable = D_80151E38;
        return -1;
    }
    count = func_8009FB00(p);
    i = 0;
    while (1) {
        if (i >= count) {
            break;
        }
        if (func_800CD278(unit->xCC) <= 0) {
            loader.vtable = D_80151E38;
            return -6;
        }
        failed = func_800AC220() != 1;
        if (failed) {
            loader.vtable = D_80151E38;
            return -7;
        }
        unit->xC8 = func_8009FB28(&loader, i);
        func_8004505C(unit->xC8, &x, &y);
        i++;
        unit->xC0 = func_80121788();
        unit->xC4 = func_800D7A24(x, y);
        func_800CD538(func_8011422C(unit->xC0), unit->xC4);
        func_800CD538(unit->xCC, unit->xC0);
    }
    loader.vtable = D_80151E38;
    return 1;
}
