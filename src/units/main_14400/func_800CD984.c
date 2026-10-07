#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef struct { s32 unk0; VtblEntry *vtbl; } Obj800CD984;
s32 func_800CD090(void *container, void *element);
s32 func_800CD764(Obj800CD984 *, s32, void *);
void func_800CD984(Obj800CD984 *self, void *element, void *replacement) {
    s32 idx = func_800CD090(self, element);
    if (idx >= 0) {
        VtblEntry *e = &self->vtbl[9];
        ((void (*)(void *, s32))e->fn)((char *)self + e->delta, idx);
        func_800CD764(self, idx, replacement);
    }
}
