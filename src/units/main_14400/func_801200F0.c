#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    void *fn;
} VtblEntry;

typedef struct {
    u32 pad : 28;
    u32 calm : 1;
    u32 low : 3;
} Flags_801200F0;

static inline s32 flags_calm(Flags_801200F0 *flags) {
    return flags->calm;
}

typedef struct {
    u8 pad0[0x20];
    Flags_801200F0 flags_20;
    VtblEntry *vtbl_24;
} Obj_801200F0;

typedef struct {
    void *source;
    u32 kind, auxiliary;
    unsigned short amount, options;
    u8 scale;
} Text_801200F0;

typedef s32 (*ReactFn)(void *self, s32 a, s32 b, u8 c, s32 d);
typedef void (*CryFn)(void *self, s16 a);

extern s16 D_80156A24;
extern s16 D_80156A2E;
extern s16 D_80156A30;
extern u8 D_80147620[];
extern void func_800EB598(Obj_801200F0 *obj, s16 a, s16 b);
extern u8 func_800C57A0(void *rng);
extern s32 func_800E0AB4(Obj_801200F0 *obj, s32 mood);
extern s32 func_800E0F40(Obj_801200F0 *obj);
extern void func_80136910(Text_801200F0 *text, void *a, u32 b, u32 c, u32 d);
extern void func_800A7ADC(Obj_801200F0 *obj, Text_801200F0 *text);

void func_801200F0(void *self /* unused callback receiver */, Obj_801200F0 *obj) {
    Text_801200F0 text;
    Text_801200F0 *textp;
    VtblEntry *entry;
    Flags_801200F0 flags;
    u8 roll;

    func_800EB598(obj, D_80156A24, D_80156A2E);
    roll = func_800C57A0(D_80147620);
    if (roll < 0x33) {
        func_800E0AB4(obj, -3);
    } else if (roll < 0x66) {
        entry = &obj->vtbl_24[18];
        ((ReactFn)entry->fn)((u8 *)obj + entry->delta, 0, 0x13, 0xFE, 0);
    } else if (roll < 0x99) {
        s32 restless;
        flags = obj->flags_20;
        restless = flags_calm(&flags);
        restless ^= 1;
        if (restless) {
            entry = &obj->vtbl_24[18];
            ((ReactFn)entry->fn)((u8 *)obj + entry->delta, 0, 0xA, 0xFE, 0);
        }
    } else if (roll < 0xCC) {
        entry = &obj->vtbl_24[18];
        ((ReactFn)entry->fn)((u8 *)obj + entry->delta, 0, 0, 0xFE, 0);
    } else if ((u8)func_800E0F40(obj) >= 2) {
        entry = &obj->vtbl_24[15];
        ((CryFn)entry->fn)((u8 *)obj + entry->delta, -1);
    }
    textp = &text;
    func_80136910(textp, 0, D_80156A30, 0x1C, 8);
    func_800A7ADC(obj, textp);
}
