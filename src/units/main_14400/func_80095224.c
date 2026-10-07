#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 field_0; u8 field_4[0x10]; void *field_14; void *field_18; } Obj80095224;
extern u8 D_8014AAE4[];
void func_800486A4(void *dst, void *desc, Obj80095224 *obj);
void func_80048650(void *dst);
void func_80048794(void *dst);
void func_80095224(Obj80095224 *obj, void *item, void *def) {
    void *sub;
    obj->field_14 = item;
    if (item == def) {
        obj->field_18 = 0;
    } else {
        obj->field_18 = def;
    }
    sub = obj->field_4;
    func_800486A4(sub, D_8014AAE4, obj);
    func_80048650(sub);
    func_80048794(sub);
}
