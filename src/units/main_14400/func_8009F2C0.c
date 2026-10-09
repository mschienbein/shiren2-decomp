#include "common.h"

typedef unsigned char u8;

/* Widget-derived object: base constructor func_800953C0 (returns the object),
 * vtable pointer at +0x4C, id at +0x1D0, callback table at +0x1D4. */
typedef struct Widget8009F2C0 {
    u8 unk_00[0x4C];
    const u8 *vtable_4C;
    u8 unk_50[0x180];
    s32 id_1D0;
    const u8 *callbacks_1D4;
} Widget8009F2C0;

extern const u8 D_80151EC8[];
extern const u8 D_80152EA8[];
extern Widget8009F2C0 *func_800953C0(Widget8009F2C0 *self);

Widget8009F2C0 *func_8009F2C0(Widget8009F2C0 *self)
{
    func_800953C0(self);
    self->vtable_4C = D_80152EA8;
    self->id_1D0 = -1;
    self->callbacks_1D4 = D_80151EC8;
    return self;
}
