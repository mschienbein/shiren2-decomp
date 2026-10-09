#include "common.h"

/* Command object: base command (func_800DA970) then its own 0x30-byte vtable at +4. */
typedef struct {
    short field_00;
    const void *field_04;
} Object;

extern void *D_801476B8;
extern const unsigned char D_801584D8[];
extern Object *func_800DA970(Object *self, s32 kind, void *actor);

Object *func_800DB928(Object *self, unsigned char *unused_payload) {
    func_800DA970(self, 0x12, D_801476B8);
    self->field_04 = D_801584D8;
    return self;
}
