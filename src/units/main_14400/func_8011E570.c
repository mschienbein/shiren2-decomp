#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 pad[0x1C]; void *vtbl; u8 pad20[0x18]; } Obj;
extern s32 func_80111A20(void *, void *);
extern Obj *func_80111E08(Obj *o, void *unit, void *item, void *pos, u8 *dir);
extern void func_800C2D0C(Obj *);
extern u8 D_8015EF30[];
void func_8011E570(void *a, u8 *b){
    Obj tmp;
    if (func_80111A20(a, b)) {
        Obj *t = &tmp;
        func_80111E08(t, b, a, b, b + 8);
        t->vtbl = D_8015EF30;
        func_800C2D0C(t);
    }
}
