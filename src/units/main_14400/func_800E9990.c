#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[0x1E]; u8 flags; } Obj;
extern Obj *D_801476B8;
void func_80049A04(u16 message_id, ...);
void func_800E9A68(Obj *, s32);
s32 func_800A8FC8(s32 *, s32);
Obj *func_800A910C(s32 *);
s32 func_800A44F4(Obj *, Obj *);
void func_800E9990(Obj *self, s32 arg, s32 announce) {
    s32 it;
    Obj *o;
    Obj *target;
    if (announce) {
        func_80049A04(arg >= 0 ? 0x4B : 0x4C, arg);
    }
    func_800E9A68(D_801476B8, arg);
    it = 0;
    while (func_800A8FC8(&it, 12)) {
        o = func_800A910C(&it);
        target = o;
        if ((o->flags >> 2) & 1) continue;
        if (o == self || func_800A44F4(self, o) == 1) {
            func_800E9A68(target, arg);
        }
    }
}
