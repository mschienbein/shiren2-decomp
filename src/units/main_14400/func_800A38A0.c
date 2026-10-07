#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x24]; void *vtable_24; } Obj800A38A0;
extern u8 D_801535E8[];
void func_800A59A4(Obj800A38A0 *obj);
s32 func_80049CB4(s32 kind, ...);
void func_800A3918(Obj800A38A0 *obj);

void func_800A38A0(Obj800A38A0 *obj, s32 flags) {
    obj->vtable_24 = D_801535E8;
    func_800A59A4(obj);
    func_80049CB4(0x87, obj);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
