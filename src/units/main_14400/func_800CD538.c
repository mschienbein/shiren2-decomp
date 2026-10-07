#include "common.h"
typedef short s16;
typedef unsigned short u16;
typedef struct { short delta; short index; void (*fn)(void *, s32); } VEntry;
typedef struct { char pad[0x8]; VEntry *vtbl; } Obj;
void *func_800CDAF4(void *, Obj *);
u16 func_800AE710(void *obj);
void func_800AE754(void *, s16);
s32 func_800CD5C0(void *, Obj *);
s32 func_800CD538(void *self, Obj *obj) {
    void *x = func_800CDAF4(self, obj);
    if (x) {
        func_800AE754(x, (s16)func_800AE710(obj));
        if (obj) {
            VEntry *e = &obj->vtbl[1];
            e->fn((char *)obj + e->delta, 3);
        }
        return 1;
    }
    return func_800CD5C0(self, obj);
}
