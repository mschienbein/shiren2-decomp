#include "common.h"

typedef unsigned char u8;
extern char D_8015F938[];
typedef struct { char pad0[0x9C]; u8 unk9C; } Child;
typedef struct { char pad0[8]; void *unk8; } Obj;
Child *func_801217DC(Obj *);
void func_8011414C(Obj *, s32);
void func_800AC68C(Obj *);
void func_8012171C(Obj *obj, s32 flags) {
    Child *child;
    obj->unk8 = D_8015F938;
    child = func_801217DC(obj);
    if (child != 0) {
        child->unk9C = 0xFF;
    }
    func_8011414C(obj, 0);
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
