#include "common.h"

typedef struct { short delta; short index; void (*pfn)(void *, s32); } VEntry;
typedef struct { char pad[0x18]; VEntry e; } VTable;
typedef struct { s32 x0; VTable *vtbl; } Base;
typedef struct { char pad[0xC]; Base b; } S;
static inline void vcall(Base *b, s32 value) { VTable *vt = b->vtbl; vt->e.pfn((char *)b + vt->e.delta, value); }
void func_801154E4(S *self, s32 value) { vcall(&self->b, value); }
