#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 bytes[4];
} Bytes4;

/* Command object: base command (func_800DA904 consumes params[0]); params[1..4] kept at +0x10. */
typedef struct {
    short field_00;
    const void *field_04;
    void *link_08[2];
    Bytes4 field_10;
} Object;

extern const unsigned char D_80158748[];
extern Object *func_800DA904(Object *object, s32 kind, u8 *params);

Object *func_800DD0F4(Object *self, u8 *params) {
    func_800DA904(self, 0x20, params++);
    self->field_04 = D_80158748;
    self->field_10 = *(Bytes4 *)params; /* four unaligned parameter bytes */
    return self;
}
