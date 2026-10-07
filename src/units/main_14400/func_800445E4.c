#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x18];
    void *field_18;
} Obj_800445E4;

extern u8 D_8014A8E8[];
extern void *func_800CA030(Obj_800445E4 *obj);

Obj_800445E4 *func_800445E4(Obj_800445E4 *obj) {
    func_800CA030(obj);
    obj->field_18 = D_8014A8E8;
    return obj;
}
