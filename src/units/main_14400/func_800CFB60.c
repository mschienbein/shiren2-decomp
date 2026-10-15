#include "common.h"
#include "item_records.h"
typedef struct { s32 field_00, field_04; } Pair;
typedef struct { ItemPool *field_00; const void *field_04; Pair field_08; } Object;
extern const unsigned char D_801544C0[132];
Object *func_800CFB60(Object *self, Pair *value) {
    self->field_04 = D_801544C0;
    self->field_00 = &D_80143094;
    self->field_08 = *value;
    return self;
}
