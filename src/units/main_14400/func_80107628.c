#include "common.h"

typedef struct Item Item;
typedef struct { s32 kind; void *sender; Item *item; s32 field0C; s32 field10; } Message;
typedef struct { unsigned char pad00[0x58]; short adjust58; short pad5A; s32 (*dispatch5C)(void *, Message *); } VTable;
typedef struct { unsigned char pad00[0x24]; VTable *vtable24; unsigned char pad28[0x30]; void *value58; unsigned char pad5C[0x3E]; unsigned short flags9A; } Obj80100160;
extern s32 func_800F10F8(Obj80100160 *obj, void *arg1, s32 arg2, s32 arg3, s32 arg4);
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Item *func_8011D3A0(Item *obj);
extern s32 func_800AC670(Item *obj);

static inline Message *make_message(Message *msg, Item *item)
{
    msg->kind = 14;
    msg->item = item;
    msg->field10 = 0;
    return msg;
}

/* Monster action slot +0xB4 supplies a target pointer; this override ignores it. */
s32 func_80107628(Obj80100160 *obj, void *target)
{
    Item *item;
    Message msg;
    Message *message;
    void *value = obj->value58;
    s32 hidden = obj->flags9A & 0x40;
    switch (func_800F10F8(obj, value, 0, hidden != 0, 0)) {
        case 1: return 0;
        case 2: return 1;
    }
    item = func_8011D3A0(func_800AC5B4(0x18, 0));
    if (func_800AC670(item)) return 1;
    message = make_message(&msg, item);
    return obj->vtable24->dispatch5C((char *)obj + obj->vtable24->adjust58, message);
}
