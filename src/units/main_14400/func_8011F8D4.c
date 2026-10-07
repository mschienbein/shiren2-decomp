#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x1C]; void *vtable; u8 pad20[0x18]; s32 unk38; } Obj8011F8D4;
extern u8 D_8015F2F8[];
void *func_80111E08(void *obj, void *arg1, void *arg2, void *pos, u8 *color);
Obj8011F8D4 *func_8011F8D4(Obj8011F8D4 *obj, void *arg1, void *arg2) {
    func_80111E08(obj, arg1, arg2, arg1, (u8 *)arg1 + 8);
    obj->vtable = D_8015F2F8;
    obj->unk38 = 0;
    return obj;
}
