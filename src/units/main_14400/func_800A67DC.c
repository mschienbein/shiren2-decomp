#include "common.h"
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { unsigned char value; } Dir;
typedef struct { Pos pos; unsigned char unk8; } Unit;
typedef struct { Pos pos; char pad8[0x1C]; VEntry *vtbl; } Obj;
u32 func_800B1C6C(Pos *);
s32 func_800A282C(Pos *, Pos *);
void *func_800A27A4(void *out, void *from, void *to);
void *func_800A6BA4(void *obj, s32 range, s32 ignore_terrain);
s32 func_800A44F4(void *self, void *target);
static inline Pos *Pos_copy(Pos *dst, Pos *src) { dst->x = src->x; dst->y = src->y; return dst; }
s32 func_800A67DC(Unit *u, Obj *o, s32 force, s32 apply) {
    if (o != 0) {
        VEntry *e = &o->vtbl[2];
        Pos p;
        Pos q;
        Pos t;
        if (((s32 (*)(char *))e->fn)((char *)o + e->delta)) return 0;
        Pos_copy(&p, &o->pos);
        if (force == 0 && (func_800B1C6C(&p) & 0x4000)) return 0;
        if (!func_800A282C(Pos_copy(&q, &u->pos), Pos_copy(&t, &p))) return 0;
        if (apply != 0) {
            unsigned char saved = u->unk8;
            Dir d;
            void *target;
            func_800A27A4(&d, &q, Pos_copy(&t, &p));
            u->unk8 = d.value;
            target = func_800A6BA4(u, 0x4C, force);
            u->unk8 = saved;
            return func_800A44F4(u, target) == 2;
        }
        return 1;
    }
    return 0;
}
