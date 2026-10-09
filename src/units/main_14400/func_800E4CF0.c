#include "common.h"
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct { u8 pad[0x48]; s32 field48, field4C; u16 field50; u8 field52, field53, field54, field55, field56; } Object;
extern s32 func_800E04D0(Object *);
extern u16 D_80158C6C[];
void func_800E4CF0(Object *obj) {
    u16 value = D_80158C6C[(u8)func_800E04D0(obj)];
    obj->field56 = 0;
    obj->field52 = 0;
    obj->field53 = 0;
    obj->field54 = 0;
    obj->field48 = 0;
    obj->field4C = 0;
    obj->field50 = value;
}
