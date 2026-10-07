#include "common.h"
typedef unsigned char u8;
typedef struct { s32 a; s32 b; } Tmp;
typedef struct { char pad0[0x54]; u8 flags; char pad55[3]; void *unk58; } Obj;
void *func_800A65E4(Tmp *, Obj *, void *);
void func_800A665C(Obj *, Tmp *);
void func_800F06E4(Obj *obj) {
    if (obj->unk58 != 0) {
        Tmp tmp;
        Tmp *p = &tmp;
        func_800A65E4(p, obj, obj->unk58);
        func_800A665C(obj, p);
    }
    obj->flags |= 4;
}
