#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj;

extern u8 D_8015DEF0[];
extern Obj *func_80116D50(Obj *obj, s32 kind);

Obj *func_80118390(Obj *obj) {
    func_80116D50(obj, 0xB);
    obj->vtable = D_8015DEF0;
    return obj;
}
