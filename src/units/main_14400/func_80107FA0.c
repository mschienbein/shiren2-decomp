#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Vec80107FA0;
typedef struct {
    Vec80107FA0 pos;
    char pad8[0x50];
    Vec80107FA0 *target;
    char pad5C[8];
    Vec80107FA0 dest;
    char pad6C[0x1D];
    u8 speed;
    char pad8A[0x32];
    s32 waiting;
    s32 waitFlag;
    s32 waitTimer;
} Actor80107FA0;
s32 func_800E1CD4(Actor80107FA0 *, s32);
s32 func_800E8350(Actor80107FA0 *);
s32 func_80107E40(Actor80107FA0 *);
void func_800F06E4(Actor80107FA0 *);
s32 func_800F17A8(Actor80107FA0 *, Vec80107FA0 *, s32);
void *func_800B36C4(void *out, void *pos, u8 kind, s32 arg3);
s32 func_800E7AA8(Actor80107FA0 *, s32);
s32 func_800E7104(Actor80107FA0 *);
u8 func_800A6420(Actor80107FA0 *, Vec80107FA0 *);
s32 func_800A650C(Actor80107FA0 *, Vec80107FA0 *);
s32 func_800A251C(Vec80107FA0 *, Vec80107FA0 *);
s32 func_800E66EC(Actor80107FA0 *);

s32 func_80107FA0(Actor80107FA0 *self) {
    Vec80107FA0 pos;
    Vec80107FA0 dest;
    Vec80107FA0 next;
    Vec80107FA0 *p;
    s32 moving;
    Vec80107FA0 *target;

    if (func_800E1CD4(self, 0x10)) {
        return func_800E8350(self);
    }
    p = &pos;
    p->x = self->pos.x;
    p->y = self->pos.y;
    if (func_80107E40(self)) {
        func_800F06E4(self);
        self->target = 0;
        self->waitFlag = 0;
        self->waitTimer = 0;
        return 0;
    }
    dest.x = self->dest.x;
    dest.y = self->dest.y;
    {
        s32 failed = func_800F17A8(self, &dest, 1) != 1;
        if (failed) {
            func_800B36C4(&next, p, 0, 1);
            if (next.y | next.x) {
                self->dest = next;
            } else {
                if (self->waiting) {
                    if (self->waitTimer == 0) {
                        self->waitTimer = 10;
                    }
                    if (--self->waitTimer == 0) {
                        self->waitFlag = 0;
                        self->waitTimer = 0;
                    }
                    return func_800E7AA8(self, (self->speed >> 1) + 1);
                }
                return func_800E7104(self);
            }
        }
    }
    moving = 0;
    dest = self->dest;
    target = self->target;
    if (func_800A6420(self, target) == 0) {
        Vec80107FA0 *d = &dest;
        if (func_800A650C(self, d) == 1) {
            moving = func_800A251C(d, target) != 0;
        }
    }
    if (moving) {
        func_800B36C4(&next, &pos, 0, 1);
        if (func_800A251C(&next, &dest)) {
            func_800F06E4(self);
            return 0;
        }
        self->dest = next;
    }
    return func_800E66EC(self);
}
