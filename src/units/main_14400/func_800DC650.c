#include "common.h"
typedef unsigned char u8;
typedef struct { void *collection; void *item; } Body800DC650;
typedef struct { short field_0; void *handler; Body800DC650 base_body; Body800DC650 body; } Obj800DC650;
extern u8 D_80158628[];
Obj800DC650 *func_800DA904(Obj800DC650 *obj, s32 kind, u8 *params);
Body800DC650 *func_800D0180(Body800DC650 *body);
void func_800DAA58(u8 value, Body800DC650 *body);
Obj800DC650 *func_800DC650(Obj800DC650 *obj, u8 *params) {
    Body800DC650 *body;
    func_800DA904(obj, 0x19, params++);
    body = &obj->body;
    obj->handler = D_80158628;
    func_800D0180(body);
    func_800DAA58(*params, body);
    return obj;
}
