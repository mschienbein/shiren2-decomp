#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x98]; short offset_98; short pad_9A; u8 *(*method_9C)(u8 *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Actor;
typedef struct { u8 pad_0[0xA8]; u8 field_A8[0x24]; u8 *field_CC; void *field_D0; } Obj;
extern Actor *D_801476B8;
extern void func_800D03B4(u8 *record, void *entries, s32 capacity);
void func_800D84C0(Obj *obj) {
    Actor *actor = D_801476B8;
    obj->field_D0 = 0;
    obj->field_CC = actor->field_24->method_9C((u8 *)actor + actor->field_24->offset_98);
    func_800D03B4(obj->field_A8, obj, 0x15);
}
