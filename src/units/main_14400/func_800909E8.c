#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x1C];
    void *item1C;
} Obj_800909E8;

void func_80091544(void *item);

void func_800909E8(Obj_800909E8 *obj) {
    func_80091544(obj->item1C);
}
