#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    void *fn;
} VtblEntry;

typedef struct {
    u8 pad0[0x8];
    VtblEntry *vtbl8;
} Obj_80118B9C;

extern VtblEntry D_80153AA0[];

void func_800AC68C(void *a);

/* Destructor body: restore the base vtable, free when bit 0 of the flags is set. */
void func_80118B9C(Obj_80118B9C *obj, s32 flags) {
    obj->vtbl8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
