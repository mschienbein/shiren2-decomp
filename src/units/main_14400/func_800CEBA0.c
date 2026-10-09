#include "common.h"
typedef short s16;
typedef struct { char pad0[0x20]; s16 offset20; s16 pad22; s32 (*call24)(void *); char pad28[0x10]; s16 offset38; s16 pad3A; void *(*call3C)(void *, u32); } Table;
typedef struct { void *pool; Table *field4; } Container;
typedef struct { s32 index; Container *owner; s32 reverse; void *current; } Iterator;
s32 func_800CEBA0(Iterator *it) {
    if (it->reverse) { for (;;) { Container *p; Table *t; if (it->index < 0) return 0; p = it->owner; t = p->field4; it->current = t->call3C((char *)p + t->offset38, (u32)it->index); if (it->current) break; --it->index; } return 1; }
    else { for (;;) { Container *p = it->owner; Table *t = p->field4; s32 size = t->call24((char *)p + t->offset20); if (it->index >= size) break; p = it->owner; t = p->field4; it->current = t->call3C((char *)p + t->offset38, (u32)it->index); if (it->current) return 1; ++it->index; } }
    return 0;
}
