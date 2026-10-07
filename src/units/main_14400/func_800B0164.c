#include "common.h"

typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[0x28]; VEntry e; } VTable;
typedef struct { char pad[0x18]; VTable *vtbl; } B;
typedef struct { char pad[8]; VTable *vtbl; } O;
/* Fixed 0x30-byte records; their contents belong to the item constructors. */
typedef struct { s32 words[0x30 / 4]; } PoolRecord;
typedef struct { PoolRecord *storage; unsigned char *bits; s32 count; } A;
typedef void (*Fn3)(void *, s32, void *);
typedef void (*FnO)(void *, B *);
extern char D_80153B00[];
extern char D_80147620[];
extern PoolRecord *D_80143104;
extern unsigned char D_8015488C[];
void func_800CA4E8(B *, void *);
void func_800C56D4(void *);
void func_800C573C(void *);
O *func_800AC244(unsigned char);
void func_800B0164(A *a, B *b) {
    s32 i;
    VEntry *e;
    func_800CA4E8(b, D_80153B00);
    e = &b->vtbl->e;
    ((Fn3)e->fn)((char *)b + e->delta, (a->count + 7) / 8, a->bits);
    i = 0;
    func_800C56D4(D_80147620);
    for (;; i++) {
        s32 n = a->count;
        if (i >= n) break;
        if (a->bits[i >> 3] & D_8015488C[i & 7]) {
            unsigned char id;
            O *o;
            ((Fn3)b->vtbl->e.fn)((char *)b + b->vtbl->e.delta, 1, &id);
            /* local-arithmetic-qualification: a->storage + i emits the
             * reversed addu operands at 0x800B0234 (v0,s1 rather than s1,v0).
             * Only this address calculation uses an integer; storage and
             * the published record override remain typed pointers. */
            D_80143104 = (PoolRecord *)(i * sizeof(PoolRecord) + (u32)a->storage);
            o = func_800AC244(id);
            ((FnO)o->vtbl->e.fn)((char *)o + o->vtbl->e.delta, b);
        }
    }
    D_80143104 = 0;
    func_800C573C(D_80147620);
}
