#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_80160388[];
extern Obj *func_80115690(Obj *obj, s32 kind);

Obj *func_80125D10(Obj *obj) {
    func_80115690(obj, 0xE4);
    obj->vtable = D_80160388;
    return obj;
}
