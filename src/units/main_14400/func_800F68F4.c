#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern const unsigned char D_80159AA0[32];
/* The paired dispatch callback reads +4 as the target and +8 as its name. */
typedef struct { const void *table; void *a; const char *b; s32 c; } Obj800F68F4;
Obj800F68F4 *func_800F68F4(Obj800F68F4 *obj, void *a, const char *b, s32 c) {
    obj->table = D_80159AA0;
    obj->a = a;
    obj->b = b;
    obj->c = c;
    return obj;
}
