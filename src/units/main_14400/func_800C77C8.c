#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; s32 (*fn)(void *self, void *msg); } VEntry;

typedef struct { s32 x0; s32 x4; } Iter;
typedef struct { s32 kind; s32 pad[5]; } Msg;
typedef struct { char pad[0x24]; VEntry *vt24; } Obj;
s32 func_800A8FC8(Iter *it, s32 kind);
Obj *func_800A910C(Iter *it);
void func_800C77C8(void) {
    Msg msg;
    Iter it;
    Obj *o;
    it.x0 = 0;
    msg.kind = 4;
    while (1) {
        if (!func_800A8FC8(&it, 0xC)) break;
        o = func_800A910C(&it);
        o->vt24[11].fn((char *)o + o->vt24[11].delta, &msg);
    }
}
