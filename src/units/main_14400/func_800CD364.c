#include "common.h"

typedef struct {
    short delta;
    short index;
    void (*func)(void *self, s32 value);
} VtableEntry;

typedef struct {
    unsigned char pad0[4];
    VtableEntry *vtable;
} ObjA;

typedef struct {
    unsigned char pad0[8];
    VtableEntry *vtable;
} ObjB;

extern s32 func_800CD090(ObjA *a, ObjB *b);

void func_800CD364(ObjA *a, ObjB *b) {
    s32 index = func_800CD090(a, b);

    if (index >= 0) {
        a->vtable[9].func((unsigned char *)a + a->vtable[9].delta, index);
        if (b != 0) {
            b->vtable[1].func((unsigned char *)b + b->vtable[1].delta, 3);
        }
    }
}
