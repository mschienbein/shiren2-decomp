#include "common.h"

typedef struct {
    unsigned char pad0[8];
    void *vtable;
} Obj;

extern unsigned char D_80153AA0[];
extern void func_800AC68C(Obj *obj);

void func_8011FE00(Obj *obj, s32 flags) {
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
