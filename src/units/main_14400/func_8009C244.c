#include "common.h"

typedef unsigned char u8;

/* Widget-derived object: base constructor func_800953C0 (returns the object),
 * vtable pointer at +0x4C, id at +0x5C, callback table at +0x60. */
typedef struct Widget8009C244 {
    u8 unk_00[0x4C];
    const u8 *vtable_4C;
    u8 unk_50[0xC];
    s32 id_5C;
    const u8 *callbacks_60;
} Widget8009C244;

extern const u8 D_80151EC8[];
extern const u8 D_80152800[];
extern Widget8009C244 *func_800953C0(Widget8009C244 *self);

Widget8009C244 *func_8009C244(Widget8009C244 *self)
{
    func_800953C0(self);
    self->vtable_4C = D_80152800;
    self->id_5C = -1;
    self->callbacks_60 = D_80151EC8;
    return self;
}
