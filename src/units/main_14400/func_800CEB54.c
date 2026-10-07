#include "common.h"
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[4]; VEntry *vtbl; } Inner;
typedef struct { s32 unk0; Inner *unk4; s32 unk8; } Obj;
static inline s32 count(Inner *i) { VEntry *e = &i->vtbl[4]; return ((s32 (*)(char *))e->fn)((char *)i + e->delta); }
void func_800CEB54(Obj *o) { o->unk0 = o->unk8 ? count(o->unk4) - 1 : 0; }
