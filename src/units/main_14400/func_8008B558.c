#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[4]; short field4; u8 pad6[8]; short fieldE; u8 pad10[0x14]; s32 field24, field28; u8 pad2C[0x30]; s32 field5C, field60, field64, field68, field6C, field70; } Object;
extern void func_80061D28(s32, s32, s32, s32);
extern void func_8007D824(s32, s32, s32, s32);
void func_8008B558(void *obj) {
    Object *object = obj;
    func_80061D28(object->field5C, object->field68, object->field5C, object->field68);
    func_8007D824(object->field64, object->field70, object->field24, object->field28);
    object->fieldE = 1;
    object->field4 = 4;
}
