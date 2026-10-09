#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* The paired dispatch callback reads +4 as the target and +8 as its name. */
typedef struct { void *vtable; void *a; const char *b; s32 c; } Obj800F69E0;
extern u8 D_80159A90[];
Obj800F69E0 *func_800F69E0(Obj800F69E0 *obj, void *a, const char *b, s32 c) {
    obj->vtable = D_80159A90;
    obj->a = a;
    obj->b = b;
    obj->c = c;
    return obj;
}
