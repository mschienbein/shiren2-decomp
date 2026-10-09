#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 delta; s16 index; void *fn; } VtblEntry800A1DAC;
typedef struct Res800A1DAC { u8 pad0[0xC]; s32 index; } Res800A1DAC;
typedef struct { u8 pad0[0x38]; s16 delta; s16 index; Res800A1DAC *(*fn)(void *self, u32 arg); } Vtbl800A1DACb;
typedef struct { u8 pad0[0x4]; Vtbl800A1DACb *vtable; } Loader800A1DAC;
typedef struct { u8 pad0[0x98]; s16 delta; s16 index; Loader800A1DAC *(*fn)(void *self); } Vtbl800A1DACa;
typedef struct { u8 pad0[0x24]; Vtbl800A1DACa *vtable; } Mgr800A1DAC;
typedef s32 (*Predicate800A1DAC)(void *object);
/* Whole 0x40C-byte menu; callback is the member at +0x2EC. */
typedef struct { u8 pad0[0x2EC]; Predicate800A1DAC callback; u8 pad2F0[0x11C]; } Ctx800A1DAC;
typedef struct { u8 pad0[0xBB]; s8 slots[12]; } Table800A1DAC;
typedef struct { Res800A1DAC *res; s32 slot; s32 status; } Out800A1DAC;
extern Mgr800A1DAC *D_801476B8;
extern Ctx800A1DAC D_801404E0;
extern s32 D_80139054[2];
extern u8 D_80154718[];
s32 func_8009A354(void *object);
s32 func_800CE46C(Loader800A1DAC *loader, Predicate800A1DAC cb);
void func_80097B90(Ctx800A1DAC *ctx, Loader800A1DAC *loader, void *holder, s32 size, s32 *arg4, s32 arg5);
s32 func_800957C0(Ctx800A1DAC *ctx, s32 *out, s32 arg2, void *arg3, s32 arg4);
Table800A1DAC *func_800C9E00(void);
s32 func_800A1DAC(Out800A1DAC *out) {
    Mgr800A1DAC *mgr = D_801476B8;
    Loader800A1DAC *loader;
    Res800A1DAC *res;
    s32 handle;
    s32 index;
    s32 slotIndex;
    s32 slot;
    s32 failed;

    out->status = 0;
    loader = mgr->vtable->fn((u8 *)mgr + mgr->vtable->delta);
    failed = func_800CE46C(loader, func_8009A354) != 1;
    if (failed) {
        return -3;
    }
    func_80097B90(&D_801404E0, loader, 0, 0x1000, D_80139054, 0);
    D_801404E0.callback = func_8009A354;
    failed = func_800957C0(&D_801404E0, &handle, 1, 0, 0) != 1;
    if (failed) {
        return -1;
    }
    res = loader->vtable->fn((u8 *)loader + loader->vtable->delta, handle);
    out->res = res;
    index = res->index;
    slotIndex = D_80154718[index];
    slot = func_800C9E00()->slots[slotIndex];
    if (slot == index) {
        return -2;
    }
    out->slot = slot;
    if (slot != -1) {
        out->status = 2;
        return -4;
    }
    out->status = 1;
    return 1;
}
