#include "common.h"
/* Eight-byte request record (collection, item); func_800DA8A0 copies it to +8. */
typedef struct { void *collection; void *item; } Request;
typedef struct { s32 field0; const void *field4; } Object;
extern const unsigned char D_80158838[48];
extern void *func_800DA8A0(Object *, s32, Request *);
Object *func_800DD590(Object *obj, Request *request) { func_800DA8A0(obj, 0x24, request); obj->field4 = D_80158838; return obj; }
