#include "common.h"
typedef struct { s32 field_0; const void *vtable_4; } Object;
extern const unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DA868(Object *p, s32 flags) { p->vtable_4 = D_80157FA8; if (flags & 1) func_800D8FE8(p); }
