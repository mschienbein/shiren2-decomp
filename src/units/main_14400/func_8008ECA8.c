#include "common.h"
typedef struct { unsigned short field_00, field_02; s32 field_04; } Reference;
typedef struct { s32 field_00; u32 field_04; Reference *field_08; s32 field_0c, field_10; } Group;
typedef struct Item { unsigned char field_00[0x10]; void (*field_10)(struct Item *); } Item;
typedef struct { unsigned char field_00[0x14]; Group *field_14; u32 field_18; Item **field_1c; } Object;
extern void func_80091544(Item *);
void func_8008ECA8(Object *self, s32 index) {
    Group *group = &self->field_14[index];
    Reference *refs = group->field_08;
    u32 i, j;
    Reference *cursor;
    for (i = 0; i < self->field_18; i++) {
        for (j = 0; j < group->field_04; j++) {
            cursor = &refs[j];
            if (i == cursor->field_02) break;
        }
        if (j >= group->field_04) {
            Item *item = self->field_1c[i];
            item->field_10(item);
            func_80091544(self->field_1c[i]);
            self->field_1c[i] = 0;
        }
    }
}
