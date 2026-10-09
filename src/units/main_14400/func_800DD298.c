#include "common.h"

typedef unsigned char u8;

/* Command object: base command (func_800DA904 consumes params[0]); params[1] is kept at +0x10. */
typedef struct {
    short field_00;
    const void *field_04;
    void *link_08[2];
    u8 field_10;
} Object;

extern const unsigned char D_80158778[];
extern Object *func_800DA904(Object *object, s32 kind, u8 *params);

Object *func_800DD298(Object *self, u8 *params) {
    func_800DA904(self, 0x1F, params++);
    self->field_04 = D_80158778;
    self->field_10 = *params;
    return self;
}
