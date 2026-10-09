#include "common.h"
/* Collection/item source link; func_800DA8A0 copies it to +8. */
typedef struct { void *collection; void *item; } Link;
typedef struct { s32 field0; const void *field4; } Obj;
extern const unsigned char D_80158808[48];
extern void *func_800DA8A0(Obj *, s32, Link *);
Obj *func_800DD4C0(Obj *p, Link *source) { func_800DA8A0(p, 35, source); p->field4 = D_80158808; return p; }
