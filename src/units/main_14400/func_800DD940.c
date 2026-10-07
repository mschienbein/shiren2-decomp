#include "common.h"

typedef unsigned char u8;

extern u8 D_80157FA8[];
extern void func_800D8FE8(void *obj);

typedef struct {
    s32 field0;
    void *vtable4;
} Obj800DD940;

void func_800DD940(Obj800DD940 *obj, s32 flags) {
    obj->vtable4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
