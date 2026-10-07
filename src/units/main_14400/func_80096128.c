#include "common.h"

extern s32 D_80151EC8;
typedef struct { void *vtbl; } Obj;
Obj *func_80096128(Obj *obj) { obj->vtbl = &D_80151EC8; return obj; }
