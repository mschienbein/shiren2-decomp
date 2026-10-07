#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8];
    void *field_8;
} Obj_801187D0;

extern u8 D_8015E050[];
extern Obj_801187D0 *func_80116D50(Obj_801187D0 *obj, s32 kind);

Obj_801187D0 *func_801187D0(Obj_801187D0 *obj) {
    func_80116D50(obj, 0xF);
    obj->field_8 = D_8015E050;
    return obj;
}
