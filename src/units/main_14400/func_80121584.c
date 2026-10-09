#include "common.h"
typedef unsigned char u8;
typedef struct { char fields_0[8]; short adjustment_8; void (*method_C)(void *, s32); } Destructor;
typedef struct { char fields_0[0x20]; short adjustment_20; s32 (*method_24)(void *); } Query;
/* List subobject: owner pointer, then its method table. */
typedef struct { void *owner_0; Query *field_4; } Subobject;
typedef struct { char fields_0[0x38]; short adjustment_38; void *(*method_3C)(void *, u32); } Lookup;
typedef struct { void *owner_0; Lookup *field_4; } Collection;
typedef struct { s32 fields_0[2]; Destructor *field_8; Subobject field_C; } Object;
typedef struct { s32 x, y; } Position;
/* Message header shared with func_80114E28; kind 0xE carries a map position at +0x10. */
typedef struct { s32 kind; u8 pad_04[0xC]; Position position_10; } Event;
extern void func_800ACDD8(Object *);
extern Collection *func_8011422C(Object *);
extern s32 func_800A8A50(void);
extern void func_801146A4(Object *, void *, unsigned short);
extern void func_800D3650(Object *);
extern s32 func_80114E28(Object *, Event *);
s32 func_80121584(Object *object, Event *event) {
    if (event->kind == 0xE) {
        Subobject *part;
        s32 message;
        func_800ACDD8(object);
        part = &object->field_C;
        if (part->field_4->method_24((char *)part + part->field_4->adjustment_20)) {
            Collection *collection = func_8011422C(object);
            u8 kind = ((u8 *)collection->field_4->method_3C((char *)collection + collection->field_4->adjustment_38, 0))[1];
            if (kind == 0xF1) {
                message = func_800A8A50() > 0 ? 0xC0 : 0xC2;
            } else {
                message = 0xBF;
                if (kind == 0xF3) message = 0xC1;
            }
        } else message = 0xC2;
        func_801146A4(object, &event->position_10, (unsigned short)message);
        func_800D3650(object);
        if (object) object->field_8->method_C((char *)object + object->field_8->adjustment_8, 3);
        return 1;
    }
    return func_80114E28(object, event);
}
