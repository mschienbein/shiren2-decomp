#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *obj);
} VEntry;
typedef struct {
    u8 pad00[0x20];
    VEntry e20;
} VTable;
typedef struct {
    s32 unk0;
    VTable *vt;
} Obj;

typedef struct S S;
struct S {
    Pos pos;
    u8 pad08[0x58 - 0x08];
    S *target58;
    u8 pad5C[0x89 - 0x5C];
    u8 range89;
    u8 pad8A[2];
    Obj *obj8C;
    u8 pad90[0x9A - 0x90];
    u16 flags9A;
    u8 pad9C[0xC0 - 0x9C];
    s32 xC0;
    s32 xC4;
};

static inline void pos_copy(Pos *dst, Pos *src)
{
    dst->x = src->x;
    dst->y = src->y;
}

extern s32 func_800A692C(S *, s32);
extern u8 func_800A6420(S *obj, S *target);
extern s32 func_800A67DC(S *u, S *o, s32 force, s32 apply);
extern s32 func_800A23E8(Pos *origin, Pos *vec);
extern void func_800F06E4(S *obj);
extern s32 func_800E1CD4(S *, s32);
extern s32 func_800E8350(S *);
extern s32 func_800E7AA8(S *obj, s32 a1);
extern s32 func_800E7104(S *unit);

s32 func_801081A4(S *self)
{
    Pos home;
    Pos goal;
    S *target;
    u8 rel;
    s32 near;

    if (func_800A692C(self, 0x12) == 0) {
        if (self->xC4 != 0 && --self->xC4 == 0) {
            self->xC0 = 1;
            self->xC4 = 0;
        }
        pos_copy(&home, &self->pos);
        target = self->target58;
        rel = func_800A6420(self, target);
        if (rel == 0 && self->xC4 == 0) {
            self->xC4 = 10;
        }
        if (rel != 3 && self->obj8C->vt->e20.fn((u8 *)self->obj8C + self->obj8C->vt->e20.delta)) {
            s32 flagged;

            near = 0;
            pos_copy(&goal, &target->pos);
            flagged = self->flags9A & 0x40;
            if (func_800A67DC(self, target, near, flagged != 0)) {
                Pos to;

                pos_copy(&to, &goal);
                near = func_800A23E8(&home, &to) <= self->range89;
            }
            if (near) {
                func_800F06E4(self);
                return 0;
            }
            if (func_800E1CD4(self, 0x10)) {
                return func_800E8350(self);
            }
            return func_800E7AA8(self, 2);
        }
    }
    if (func_800E1CD4(self, 0x10)) {
        return func_800E8350(self);
    }
    return func_800E7104(self);
}
