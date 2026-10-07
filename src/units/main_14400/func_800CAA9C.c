#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[7]; u8 x7[3]; u8 padA[0x12]; void *x1C; } Obj;
void func_800CA088(void *a);
void func_800CA2C0(void *a);
void func_800CAD44(Obj *obj);
void func_800CAA9C(Obj *obj) {
    u8 *p;
    s32 i;
    func_800CA088(obj->x1C);
    func_800CA2C0(obj->x1C);
    p = obj->x7;
    i = 2;
    do {
        *p++ = 0;
    } while (i-- > 0);
    func_800CAD44(obj);
}
