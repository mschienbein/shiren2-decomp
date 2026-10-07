#include "common.h"

typedef struct { short delta; short index; void (*fn)(void *, s32, void *); } VEntry;
typedef struct { char pad[0x18]; VEntry e; } VTable;
typedef struct { char pad[0x18]; VTable *vtbl; } B;
typedef struct { char pad[0xC]; char xC[0x1C]; unsigned char x28; unsigned char x29; } A;
extern char D_8015D934[];
void func_800AF11C(A *, B *);
void func_800CA4A4(B *, void *);
void func_800D0064(void *, B *);
void func_801152EC(A *a, B *b) {
    func_800AF11C(a, b);
    func_800CA4A4(b, D_8015D934);
    func_800D0064(a->xC, b);
    b->vtbl->e.fn((char *)b + b->vtbl->e.delta, 1, &a->x28);
    b->vtbl->e.fn((char *)b + b->vtbl->e.delta, 1, &a->x29);
}
