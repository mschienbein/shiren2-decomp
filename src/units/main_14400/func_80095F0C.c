#include "common.h"

typedef unsigned char u8;
/* Widget vtable view: slot 14 (+0x70 this-adjust, +0x74 method) returns the child
 * widget of a flattened item index (null = none). */
typedef struct {
    u8 field_00[0x70];
    short field_70;
    short field_72;
    void *(*field_74)(void *, s32);
} Methods;
typedef struct {
    u8 field_00[0x20];
    s32 field_20;
    u8 field_24[0x10];
    s32 field_34;
    s32 field_38;
    u8 field_3C[0x10];
    Methods *field_4C;
} Object;

/* Child widget of the item under the cursor (0x34 column, 0x38 row). */
void *func_80095F0C(Object *object) {
    return object->field_4C->field_74((u8 *)object + object->field_4C->field_70,
        object->field_34 + object->field_20 * object->field_38);
}
