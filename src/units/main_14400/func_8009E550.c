#include "common.h"

typedef unsigned char u8;

/* Widget-derived object: base constructor func_800953C0 (returns the object),
 * vtable pointer at +0x4C, id at +0x64, callback table at +0x68. */
typedef struct Widget8009E550 {
    u8 unk_00[0x4C];
    const u8 *vtable_4C;
    u8 unk_50[0x14];
    s32 id_64;
    const u8 *callbacks_68;
} Widget8009E550;

extern const u8 D_80151EC8[];
extern const u8 D_80152D68[];
extern Widget8009E550 *func_800953C0(Widget8009E550 *self);

Widget8009E550 *func_8009E550(Widget8009E550 *self)
{
    func_800953C0(self);
    self->vtable_4C = D_80152D68;
    self->id_64 = -1;
    self->callbacks_68 = D_80151EC8;
    return self;
}
