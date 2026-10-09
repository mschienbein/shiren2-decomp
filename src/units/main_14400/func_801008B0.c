#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 kind; void *source; void *item; s32 fieldC; s32 field10; } Message;
typedef struct { u8 pad0[0x58]; short delta58, pad5A; s32 (*dispatch5C)(void *, Message *); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vtable; u8 pad28[0x30]; void *field58; u8 pad5C[0x3E]; u16 field9A; } Object;
extern s32 func_800F10F8(Object *object, void *target, s32 a, s32 b, s32 c);
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_8011D400(void *object);
extern s32 func_800AC670(void *object);
static __inline__ s32 has_flag(Object *object, s32 mask) {
    return (object->field9A & mask) != 0;
}
static __inline__ void *target(Object *object) { return object->field58; }
static __inline__ void initialize(Message *message, void *item) {
    message->kind = 0xE;
    message->item = item;
    message->field10 = 0;
}
/* Actor vtable slot 0xB4 override (D_8015B2A8+0xB4). Slot callers (e.g. func_800F27A4 at
 * 0x800F2B28) pass the receiver and a target in a1; this override never reads the target. */
s32 func_801008B0(Object *object, void *slot_target) {
    Message message;
    void *item;
    s32 result = func_800F10F8(object, target(object), 0, has_flag(object, 0x40), 0);
    if (result != 1) {
        if (result != 2) {
            item = func_8011D400(func_800AC5B4(0x18, 0));
            if (func_800AC670(item)) return 0;
            initialize(&message, item);
            object->vtable->dispatch5C((char *)object + object->vtable->delta58, &message);
        }
        return 1;
    }
    return 0;
}
