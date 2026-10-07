#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x4C];
    void *field_4C;
} Obj_8009F644;

extern u8 D_80152FD8[];
extern Obj_8009F644 *func_800953C0(Obj_8009F644 *obj);

Obj_8009F644 *func_8009F644(Obj_8009F644 *obj) {
    func_800953C0(obj);
    obj->field_4C = D_80152FD8;
    return obj;
}
