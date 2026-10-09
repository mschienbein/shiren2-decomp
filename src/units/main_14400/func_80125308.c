#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x8]; const void *vtbl; } Obj;
extern const s32 D_80153AA0[];
void func_800AC68C(void *ptr);
void func_80125308(Obj *obj, s32 flags) {
    obj->vtbl = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
