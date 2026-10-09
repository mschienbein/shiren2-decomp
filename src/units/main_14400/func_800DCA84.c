#include "common.h"

/* Command object: base command (func_800DA970) then its own 0x30-byte vtable at +4. */
typedef struct {
    short field_00;
    const void *field_04;
} Object;

extern void *D_801476B8;
extern const unsigned char D_80158688[];
extern Object *func_800DA970(Object *self, s32 kind, void *actor);

Object *func_800DCA84(Object *self, unsigned char *unused_payload) {
    func_800DA970(self, 0x1B, D_801476B8);
    self->field_04 = D_80158688;
    return self;
}
