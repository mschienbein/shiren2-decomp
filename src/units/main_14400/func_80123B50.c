#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj_80123B50;

extern u8 D_8015FCE8[];
extern Obj_80123B50 *func_80115690(Obj_80123B50 *obj, s32 kind);

Obj_80123B50 *func_80123B50(Obj_80123B50 *obj) {
    func_80115690(obj, 0xD0);
    obj->field_8 = D_8015FCE8;
    return obj;
}
