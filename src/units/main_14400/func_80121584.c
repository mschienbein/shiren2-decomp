#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef struct { char fields_0[0x38]; short adjustment_38; void *(*method_3C)(void *, u32); } Lookup;
typedef struct { void *owner_0; Lookup *field_4; } Collection;
typedef struct { s32 x, y; } Position;
/* Message header shared with func_80114E28; kind 0xE carries a map position at +0x10. */
typedef struct { s32 kind; u8 pad_04[0xC]; Position position_10; } Event;
extern void func_800ACDD8(Item *);
extern Collection *func_8011422C(Item *);
extern s32 func_800A8A50(void);
extern void func_801146A4(Item *, void *, unsigned short);
extern void func_800D3650(Item *);
extern s32 func_80114E28(Item *, Event *);
s32 func_80121584(Item *object, Event *event) {
    if (event->kind == 0xE) {
        ItemList *part;
        s32 message;
        func_800ACDD8(object);
        part = &object->payload.pot.list;
        if (part->methods->count24((char *)part + part->methods->delta20)) {
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
        if (object) object->field08->method0C((char *)object + object->field08->delta08, 3);
        return 1;
    }
    return func_80114E28(object, event);
}
