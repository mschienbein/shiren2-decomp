#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 v; } Dir;
/* Item method table at +8; slot 8 (+0x40 delta, +0x44 pfn) is the stat getter. */
typedef struct { short delta; short index; u8 (*stat)(void *self, s32 id); } VEntry;
typedef struct { char pad[0x8]; VEntry *vtbl; } Obj;
typedef struct { void *source; u32 kind; u32 field_8; u16 amount; u16 flags; u8 field_10; } Damage;
s32 func_800B5900(void *pos, s32 mode, s32 arg, void **out);
void func_800A2758(Pos *p, Dir d);
u32 func_800B1C6C(Pos *pos);
s32 func_80049CB4(s32 id, ...);
void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 flags);
void func_800A7ADC(void *target, Damage *damage);
/* Damage record initializer: func_80136910 with kind 6 and flags 0x808. */
static inline void init_damage(Damage *damage, void *source, u32 amount) { func_80136910(damage, source, amount, 6, 0x808); }
static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static inline Dir Dir_opposite(Dir *d) {
    Dir r;
    r.v = (d->v + 4) & 7;
    return r;
}
void func_8010F9FC(Obj *self, void *name, Dir dir, void *actor, Pos *start) {
    Pos pos;
    void *target;
    VEntry *e;
    s32 kind;
    Pos *p;
    Pos_copy(&pos, start);
    while (1) {
        Pos t;
        s32 failed;
        Pos_copy(&t, &pos);
        failed = func_800B5900(&t, 2, 2, &target) != 1;
        if (failed) {
            /* ODD_C: the final position pointer is taken on the exit path, so it is
               rematerialized after the loop instead of reusing the loop's pointer. */
            p = &pos;
            break;
        }
        func_800A2758(&pos, dir);
    }
    if (func_800B1C6C(p) & 0x4000) {
        func_800A2758(p, Dir_opposite(&dir));
    }
    e = &self->vtbl[8];
    kind = e->stat((char *)self + e->delta, 5);
    func_80049CB4(0x100, actor, p);
    if (target) {
        Damage damage;
        init_damage(&damage, name, kind);
        func_800A7ADC(target, &damage);
    }
}
