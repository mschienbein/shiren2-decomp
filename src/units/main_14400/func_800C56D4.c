#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; s32 (*fn)(void *); } VEntry;

typedef struct { void *x0; char *base4; s32 count8; VEntry *vtC; } S;
void func_800C5634(S *p, void *addr);
void func_800C56D4(S *p) {
    s32 size, off;
    if (p->count8 < 3) {
        size = p->vtC[5].fn((char *)p + p->vtC[5].delta);
        off = size * p->count8++;
        func_800C5634(p, p->base4 + off);
    }
}
