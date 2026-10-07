#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 pad0[0x60]; s16 x60; s16 pad62; s32 (*x64)(void *, void *, s32); } VTableA;
typedef struct { s32 x0; VTableA *vtable; } TargetA;
typedef struct { u8 pad0[0x38]; s16 x38; s16 pad3A; s32 (*x3C)(void *, void *); } VTableB;
typedef struct { u8 pad0[8]; VTableB *vtable; } Item;
typedef struct { TargetA *target; Item *item; } Link;
typedef struct { u8 pad0[8]; Link link; } Obj;
typedef struct { s32 kind; void *value; u8 pad8[0x18]; } Message;
extern void *D_801476B8;
void func_800DAD20(Obj *obj, Item *item);
void func_800DAD80(Obj *obj);
void *func_8011422C(void *obj);
void func_800CD304(void *list, u32 index);
s32 func_800DBBCC(Obj *obj) {
    Link *link = &obj->link;
    Item *item;
    Message msg;
    Message *mp;
    s32 failed = link->target->vtable->x64((u8 *)link->target + link->target->vtable->x60, link->item, 1) != 1;
    if (failed) {
        return 0;
    }
    item = link->item;
    func_800DAD20(obj, item);
    mp = &msg;
    msg.kind = 13;
    mp->value = D_801476B8;
    if (item->vtable->x3C((u8 *)item + item->vtable->x38, &msg)) {
        func_800CD304(func_8011422C(item), 0);
    }
    func_800DAD80(obj);
    return 0;
}
