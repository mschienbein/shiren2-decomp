#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void (*fn)(void *, s32, void *); } VEntry;
typedef struct { char pad0[0x18]; VEntry *vtbl; } Other;
typedef struct { char pad0[0x20]; s32 unk20; } Self;
extern s32 D_8015D3E4[];
void func_8010C3C4(Self *, Other *);
void func_800CA4A4(Other *, void *);
void func_8010EF68(Self *self, Other *other) {
    func_8010C3C4(self, other);
    func_800CA4A4(other, D_8015D3E4);
    other->vtbl[3].fn((char *)other + other->vtbl[3].delta, 2, &self->unk20);
}
