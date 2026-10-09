#include "common.h"

typedef unsigned char u8;
typedef struct Obj { u8 pad_00[0x8C]; struct Obj *field_8C; u8 pad_90[0x30]; u8 field_C0; } Obj;
typedef struct { s32 field_00; Obj *field_04; void *field_08; } Msg_800F96B4;
typedef Obj Obj_800F96B4;
typedef Obj Obj_80049414;
typedef struct { u8 value; } S;
typedef Obj T;
extern s32 func_800CD1FC(Obj *obj);
extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
extern void func_800F0CB0(Obj *obj, Obj *target);
extern void *func_800A65E4(S *p, T *q, void *target);
extern void func_800A665C(Obj *obj, u8 *value);
extern s32 func_801017A4(Obj *obj, void *request);
extern s32 func_800F27A4(Obj_800F96B4 *obj, Msg_800F96B4 *msg);
s32 func_8010197C(Obj *obj, Msg_800F96B4 *msg)
{
    S direction;
    switch (msg->field_00) {
    case 0xB:
        obj->field_C0 -= func_800CD1FC(obj->field_8C);
        break;
    case 0xF:
        if ((func_800E1CC4(obj, 2) ^ 1) == 0) {
            return 0;
        }
        func_800F0CB0(obj, msg->field_04);
        func_800A65E4(&direction, obj, msg->field_04);
        func_800A665C(obj, &direction.value);
        return func_801017A4(obj, msg->field_08);
    }
    return func_800F27A4(obj, msg);
}
