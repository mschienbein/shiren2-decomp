#include "common.h"
#include "collection_iterator.h"
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[4]; VEntry *vtbl; } Inner;
static inline s32 count(Inner *i) { VEntry *e = &i->vtbl[4]; return ((s32 (*)(char *))e->fn)((char *)i + e->delta); }
void func_800CEB54(CollectionIterator *o) { o->index = o->reverse ? count(o->owner) - 1 : 0; }
