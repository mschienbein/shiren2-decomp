#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x1C]; void *field_1C; } Obj;
void func_80091544(void *p);
void func_800905A4(Obj *o) {
    if (o->field_1C != 0) {
        func_80091544(o->field_1C);
        o->field_1C = 0;
    }
}
