#include "common.h"
typedef struct { short field_00; const void *field_04; } Object;
extern const s32 D_80157FA8[]; /* 0x30-byte command vtable. */
extern const s32 D_80158B08[12]; /* Complete 0x30-byte command vtable. */
/* The parser factory supplies its payload pointer; this action has no payload. */
Object *func_800DF9C8(Object *self, unsigned char *unused_data) {
    self->field_04 = &D_80157FA8;
    self->field_00 = 0x2f;
    self->field_04 = &D_80158B08;
    return self;
}
