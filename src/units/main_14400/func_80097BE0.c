#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef struct { u8 pad00[4]; u8 count04; u8 pad05[0xB]; u8 count10; } Entry;
/* D_801544C0 slots 0x14/0x3C target func_800CFBBC and func_800CFC2C (ROM 0x801544D4/0x801544FC; deltas 0). */
typedef struct { u8 pad00[0x10]; s16 countDelta10, countIndex12; s32 (*count14)(void *); u8 pad18[0x20]; s16 getDelta38, getIndex3A; void *(*get3C)(void *, u32); } VTable;
typedef struct { void *storage; VTable *vtable04; } Source;
typedef struct MenuPos { s32 x, y; } Pair;
typedef struct { s32 count; u32 style; Pair bounds; } Desc;
typedef struct { u8 pad00[0x44]; u8 field44; u8 pad45[0x287]; s32 flag2CC; u8 pad2D0[0x1C]; s32 field2EC; void *sources2F0[3]; s32 mode2FC; u8 pad300[0xE4]; s32 field3E4; u8 pad3E8[0x20]; s16 field408; } Object;
extern s32 D_80138E08;
extern u32 D_80138E54;
/* D_80147F98 is the +8 mode byte of the pool at 0x80147F90. */
typedef struct Pool { void *records; void *methods; s8 mode; } Pool;
extern Pool D_80147F90;
typedef struct { u8 kind, variant, field02, flags, row, field05; unsigned short field06; u8 mode; s8 coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern s32 func_800A99D0(void);
extern void func_80099E50(Object *self);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8009543C(Object *self, Desc *desc);
extern void func_80098718(Object *self);
extern void func_800957AC(Object *self, unsigned char value);

void func_80097BE0(Object *self, void **buf, s32 mode, const Pair *bounds, s32 flag) {
    s32 count;
    s32 i;
    u32 style;
    Desc copy, configuration;
    for (i = 0; i < 3; i++) self->sources2F0[i] = buf[i];
    style = D_80138E08;
    self->mode2FC = mode;
    switch (mode) {
    /* ODD_C: the ten-row modes are two separate case groups, each with its
     * own `count = 10`; mode 0x8000 clears the flag and falls into the first.
     * jump2's cross-jump (jump.c find_cross_jump: a label in the first
     * stream lowers the minimum) merges that group into the later group's
     * copy, giving the ROM's `j 0x80097E00; move s2,zero`. The second
     * `count = 10` is the reference that lets `count` outrank `allowed` in
     * global.c (10 refs: 3*10/48 > 3/5), so count takes s0 and mode/allowed
     * share s1. A goto into a single group gives the same layout with 9
     * refs (count 5869 < allowed 6000: count/allowed swap, 47 words). */
    case 0x8000: flag = 0;
    case 1: case 0x80: case 0x200: case 0x800: count = 10; break;
    case 0x400: count = 10; style = D_80138E54; break;
    case 8: case 0x4000: case 0xC000: flag = 0;
    case 4: case 0x2000: {
        Source *source = self->sources2F0[0];
        count = source->vtable04->count14((u8 *)source + source->vtable04->countDelta10);
        break;
    }
    case 0x10: {
        Source *source = self->sources2F0[0];
        Entry *entry;
        flag = 0;
        entry = source->vtable04->get3C((char *)source + source->vtable04->getDelta38, flag);
        if (!entry) {
            source = self->sources2F0[0];
            count = source->vtable04->count14((u8 *)source + source->vtable04->countDelta10);
        } else count = entry->count10;
        break;
    }
    case 2: {
        Source *source = self->sources2F0[0];
        flag = 0;
        if (!source) {
            count = 1;
        } else {
            Entry *entry = source->vtable04->get3C((char *)source + source->vtable04->getDelta38, 0);
            count = entry->count04;
        }
        break;
    }
    case 0x20: case 0x40: case 0x100: case 0x1000: count = 10; break;
    default: flag = 0; count = 1; break;
    }
    if (flag && mode != 4 && mode != 0x2000 && mode != 0x20) {
        if (D_80147F90.mode < 0) {
            s32 allowed = 0;
            if (func_800A99D0()) {
                /* ODD_C: the comparison is held in a byte-sized boolean before
                 * widening. combine folds the widening into the set and
                 * canonicalizes (gtu x 0) to (ne x 0) = sne_si_zero, whose
                 * `sltu rd,$0,rs` has a fixed $0. Assigned directly, the int
                 * comparison stays sgtu_si and reload_cse_regs replaces its
                 * zero operand with s1 (known 0): `sltu s1,s1,v0`, 1 word. */
                u8 ok = D_80142F18.mode != 0x4F;
                allowed = ok;
            }
            if (!allowed) flag = 0;
        }
    }
    self->flag2CC = flag;
    func_80099E50(self);
    self->field2EC = 0;
    self->field408 = 0;
    self->field3E4 = 0;
    func_8006A810((u8 *)&configuration, 0, sizeof(configuration));
    configuration.count = count;
    configuration.style = style;
    configuration.bounds = *bounds;
    copy = configuration;
    func_8009543C(self, &copy);
    func_80098718(self);
    self->field44 = 0;
    if (self->mode2FC == 2 && self->sources2F0[0]) func_800957AC(self, 1);
}
