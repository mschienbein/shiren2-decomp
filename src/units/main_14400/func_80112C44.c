#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef unsigned char u8;
typedef struct { char pad[0x18]; VtblEntry *vtbl; } Obj80112C44;
typedef struct { s32 unk0; u8 unk4; char pad5[7]; s32 unkC; } Self80112C44;
extern s32 D_8015D694[];
void func_800AF174(Self80112C44 *, Obj80112C44 *);
void func_800CA4E8(Obj80112C44 *, void *);
void func_80112C44(Self80112C44 *self, Obj80112C44 *obj) {
    VtblEntry *e;
    func_800AF174(self, obj);
    func_800CA4E8(obj, D_8015D694);
    e = &obj->vtbl[5];
    ((void (*)(void *, s32, void *))e->fn)((char *)obj + e->delta, 4, &self->unkC);
    self->unk4 = (self->unkC == 3) ? 2 : 1;
}
