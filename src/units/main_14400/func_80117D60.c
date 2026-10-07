#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj80117D60;

extern u8 D_8015DD38[];

Obj80117D60 *func_80116D50(Obj80117D60 *obj, s32 kind);

Obj80117D60 *func_80117D60(Obj80117D60 *obj) {
    func_80116D50(obj, 0x6);
    obj->field_8 = D_8015DD38;
    return obj;
}
