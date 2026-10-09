#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
typedef struct { s32 field_00; Obj *field_04; s32 field_08, field_0C, field_10; u8 *field_14; } Message;
typedef struct { u8 pad_00[0x58]; short delta_58; short index_5A; s32 (*method_5C)(void *, Message *); } Methods;
struct Obj { u8 pad_00[8]; u8 field_08; u8 pad_09[0x15]; u8 field_1E; u8 pad_1F[5]; Methods *field_24; u8 pad_28[0x61]; u8 field_89, field_8A; };
extern s32 func_800F1040(Obj *obj, Obj *target, s32 id);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Obj *obj);
extern void func_800497F0(s32 id, ...);
extern void func_800A7204(Obj *target, Obj *obj, u8 *dir, s32 a, s32 b, s32 c, s32 d, s32 e);
extern void func_800A665C(Obj *obj, u8 *dir);
extern void *func_800A65E4(u8 *out, Obj *obj, void *target);
extern void func_800A7BA4(Obj *obj, s32 value);
s32 func_800FEE54(Obj *obj, Obj *target)
{
    Message message;
    u8 direction;
    u8 reverse;
    s32 amount;
    Message *p;
    s32 event;
    if (func_800F1040(obj, target, 0x5F) != 2) {
        event = func_80049CB4(0x5F, obj);
        func_800497F0(0x136, event, func_800A3B20(obj));
        direction = obj->field_08;
        amount = obj->field_89;
        p = &message;
        if (amount != 0) {
            func_800A7204(target, obj, &direction, amount, 0, 6, 0, 0);
        }
        if (target->field_1E & 0x7C) {
            s32 value;
            func_800A665C(target, &direction);
            value = obj->field_8A;
            message.field_00 = 11;
            p->field_04 = obj;
            p->field_10 = value;
            target->field_24->method_5C((u8 *)target + target->field_24->delta_58, p);
            func_800A65E4(&reverse, target, obj);
            func_800A665C(target, &reverse);
        }
        if (amount != 0) {
            func_800A7BA4(target, 1);
        }
        return 1;
    }
    return 1;
}
