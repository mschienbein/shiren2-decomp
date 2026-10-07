#include "common.h"
typedef struct { short delta; short index; void (*fn)(void *self, s32 size, void *data); } VtEntry;
typedef struct { char pad[0x28]; VtEntry read28; } ReadVtable;
typedef struct { char pad[0x18]; ReadVtable *vt; } Target;
/* The serialized subobject covers +0x84..+0xB7, including the word at +0x88. */
typedef struct { s32 opaque84; s32 unk88; s32 opaque8C[11]; } SerializedBlock;
/* +0x104 is the retained actor pointer (func_800EE290); this initializer clears it. */
typedef struct { char pad[0x84]; SerializedBlock block84; char padB8[0x14]; char unkCC[0x20]; s32 unkEC; char padF0[0x14]; void *unk104; } Obj;
extern char D_80158F74[];
void func_800E9614(Obj *, Target *);
void func_800CA4E8(Target *, void *);
void func_800CE9C4(void *, Target *);
void func_800ECBC4(Obj *o, Target *t){
    func_800E9614(o, t);
    func_800CA4E8(t, D_80158F74);
    t->vt->read28.fn((char *)t + t->vt->read28.delta, 0x34, &o->block84);
    func_800CE9C4(o->unkCC, t);
    o->unkEC = o->block84.unk88;
    o->unk104 = 0;
}
