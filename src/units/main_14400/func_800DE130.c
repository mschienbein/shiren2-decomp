#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x4];
    void *field_4;
} Obj;

extern u8 D_80158958[];
void func_800DDAD0(Obj *obj, s32 kind);

Obj *func_800DE130(Obj *obj) {
    func_800DDAD0(obj, 0x29);
    obj->field_4 = D_80158958;
    return obj;
}
