#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Obj800A674C Obj800A674C;
typedef struct {
    char pad0[0x10];
    short delta_10;
    short index_12;
    s32 (*func_14)(char *self);
} VTable800A674C;

struct Obj800A674C {
    u8 pad0[9];
    u8 field_9;
    u8 padA[0x24 - 0xA];
    VTable800A674C *vtable_24;
};

s32 func_800A58B8(Obj800A674C *obj);
s32 func_800B58E4(s32 a, s32 b, s32 c, s32 d);

s32 func_800A674C(Obj800A674C *self, Obj800A674C *other) {
    s32 selfKind;
    s32 selfValue;
    s32 otherKind;

    if (other == 0 || other->vtable_24->func_14((char *)other + other->vtable_24->delta_10)) {
        return 0;
    }
    selfKind = self->field_9 & 0xF;
    selfValue = func_800A58B8(self);
    otherKind = other->field_9 & 0xF;
    return func_800B58E4(selfKind, selfValue, otherKind, func_800A58B8(other));
}
