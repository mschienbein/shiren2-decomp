#include "common.h"

extern char D_80153AA0[];
typedef struct { unsigned char x0, x1, x2, x3, x4; signed char x5; char pad[2]; void *vtbl; } S;
void func_800AD254(S *);
void *func_800AC0C0(S *self, s32 a, s32 b) {
    self->vtbl = D_80153AA0;
    self->x4 = 1; self->x3 = 2; self->x0 = a; self->x1 = b; self->x2 = 0; self->x5 = -1;
    func_800AD254(self);
    return self;
}
