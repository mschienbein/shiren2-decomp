#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Obj80122FE4 Obj80122FE4;

extern void func_8011414C(Obj80122FE4 *self, s32 flags);
extern void func_800AC68C(void *a);

void func_80122FE4(Obj80122FE4 *obj, s32 flags)
{
    func_8011414C(obj, 0);
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
