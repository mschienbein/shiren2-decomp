#include "common.h"
extern char D_80151E38[];
extern void func_800D8FA8(void *object);
typedef struct { char pad[0x4C]; void *vt; } Obj;
void func_8009C3DC(Obj *self, s32 flags){ self->vt = D_80151E38; if (flags & 1) func_800D8FA8(self); }
