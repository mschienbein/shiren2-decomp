#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1C];
    void *field_1C;
} Obj_80090058;

extern void func_80091544(void *handle);

void func_80090058(Obj_80090058 *obj) {
    if (obj->field_1C != 0) {
        func_80091544(obj->field_1C);
        obj->field_1C = 0;
    }
}
