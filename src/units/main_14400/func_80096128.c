#include "common.h"

extern const s32 D_80151EC8[5]; /* Complete 0x14-byte callback table. */
typedef struct { const void *vtbl; } Obj;
Obj *func_80096128(Obj *obj) { obj->vtbl = &D_80151EC8; return obj; }
