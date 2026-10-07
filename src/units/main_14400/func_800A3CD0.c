#include "common.h"
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { char pad[0x24]; VEntry *vtbl; } Obj;
char *func_800A3B20(Obj *);
char *func_800A3CD0(Obj *o) { char *v = func_800A3B20(o); VEntry *e = &o->vtbl[6]; return ((char *(*)(char *, char *))e->fn)((char *)o + e->delta, v); }
