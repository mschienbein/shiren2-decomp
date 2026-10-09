#include "common.h"
typedef struct { s32 field_00, field_04; } Pair;
typedef struct { void *field_00; const void *field_04; Pair field_08; } Object;
extern s32 D_80143094[]; /* Address-only view of the 0x10-byte pool. */
extern const unsigned char D_801544C0[132];
Object *func_800CFB60(Object *self, Pair *value) {
    self->field_04 = D_801544C0;
    self->field_00 = &D_80143094;
    self->field_08 = *value;
    return self;
}
