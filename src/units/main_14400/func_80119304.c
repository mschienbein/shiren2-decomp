#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair first, last; } Quad;
typedef Quad Q;
typedef struct { Pair pos; u8 pad8[0x16]; u8 flags1E; } Unit;
typedef struct { s32 index; Pair *origin; Pair pos; Quad bounds; } Obj;
typedef Obj UnitIter;
extern s8 D_80140160[];
extern u8 D_801F5228[32];
extern void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Unit *u);
extern Obj *func_800A9204(Obj *o, Quad *q, Pair *p);
extern s32 func_800A9284(UnitIter *, s32);
extern Unit *func_800A942C(UnitIter *);
/* Returns its rectangle by value through the hidden result pointer. */
extern Q func_800B3080(void *pos);
extern u16 func_800E08B0(void *obj);
extern s32 func_80118F10(void *self, Unit *source, Unit *target, s32 count);
extern s32 func_801EF340(s32 enabled, s32 mode);
extern void func_801F212C(u16 id, u8 flag);
/* ODD_C: boolean helper; GCC 2.8.1 expands its result as seq (sltiu rd,rs,1), as the
   original does, so reload_cse cannot substitute the live constant-one register. */
static inline u8 is_zero(u32 value) {
    return value == 0;
}
static inline s32 is_finished(Unit *target) {
    s32 finished = 0;
    if ((target->flags1E >> 2) & 1) finished = is_zero(func_800E08B0(target));
    return finished;
}
void func_80119304(void *self, Unit *source) {
    UnitIter iterator;
    Quad bounds;
    s32 count;
    s32 affected;
    func_800498E4(0x8A, func_800A3B20(source));
    func_80049BF0(0);
    func_80049CB4(2);
    func_801F212C(0x2C7, 0);
    func_801EF340(D_80140160[4] == 1, 0);
    count = D_801F5228[4] & 1;
    if (D_801F5228[4] & 2) count++;
    if (D_801F5228[4] & 4) count++;
    if (D_801F5228[4] & 8) count++;
    if (D_801F5228[4] & 0x10) count++;
    if (D_801F5228[4] & 0x20) count++;
    if (D_801F5228[4] & 0x40) count++;
    affected = 0;
    bounds = func_800B3080(source);
    func_800A9204(&iterator, &bounds, &source->pos);
    while (func_800A9284(&iterator, 0x7C)) {
        Unit *target = func_800A942C(&iterator);
        if (func_80118F10(self, source, target, count)) {
            affected = 1;
            if (is_finished(target)) return;
        }
    }
    if (!affected) func_80118F10(self, source, source, count);
}
