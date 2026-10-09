#include "common.h"

typedef struct VTable VTable;
typedef struct { unsigned char pad00[8]; VTable *vtable08; unsigned char values0C[20]; } S;
extern void *func_800AC0C0(S *self, s32 a, s32 b);
extern VTable D_8015D068;

S *func_8010B8D0(S *self, s32 a, s32 b)
{
    s32 count;
    unsigned char *p;
    func_800AC0C0(self, a, b);
    self->vtable08 = &D_8015D068;
    p = self->values0C;
    for (count = 19; count != -1; count--) {
        *p++ = 0;
    }
    return self;
}
