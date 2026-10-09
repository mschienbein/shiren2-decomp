#include "common.h"

typedef unsigned char u8;
typedef struct Obj800AE444 Obj800AE444;
typedef struct S S;
typedef struct VTable VTable;
typedef struct { u8 pad00[8]; const void *vtable08; u8 pad0C[4]; } Object;
extern const VTable D_80160568, D_80160640;
extern const unsigned char D_80160520[72], D_801605B0[72], D_801605F8[72];
extern u8 func_800AC1AC(u8 id);
extern void *func_800AC5F4(s32 size, Obj800AE444 *obj);
extern S *func_8010EA20(S *self, s32 id);
extern Object *func_8010CC70(Object *self, s32 id);
extern Object *func_80113520(Object *self, s32 id);
extern Object *func_801128F0(Object *self, s32 id);

void *func_800AC3CC(u8 id, Obj800AE444 *allocator) {
    u8 kind = func_800AC1AC(id);
    if (kind == 3) return func_8010EA20(func_800AC5F4(0x24, allocator), id);
    if (kind == 4) return func_8010CC70(func_800AC5F4(0x20, allocator), id);
    if (kind == 6) return func_80113520(func_800AC5F4(0x10, allocator), id);
    if (id == 0xE9) {
        Object *obj = func_800AC5F4(0x10, allocator);
        func_801128F0(obj, 0xE9);
        obj->vtable08 = D_80160520;
        return obj;
    }
    if (id == 0xEA) {
        Object *obj = func_800AC5F4(0x10, allocator);
        func_801128F0(obj, 0xEA);
        obj->vtable08 = &D_80160568;
        return obj;
    }
    if (id == 0xEB) {
        Object *obj = func_800AC5F4(0x10, allocator);
        func_801128F0(obj, 0xEB);
        obj->vtable08 = D_801605B0;
        return obj;
    }
    if (id == 0xEC) {
        Object *obj = func_800AC5F4(0x10, allocator);
        func_801128F0(obj, 0xEC);
        obj->vtable08 = D_801605F8;
        return obj;
    }
    if (id == 0xED) {
        Object *obj = func_800AC5F4(0x10, allocator);
        func_801128F0(obj, 0xED);
        obj->vtable08 = &D_80160640;
        return obj;
    }
    return 0;
}
