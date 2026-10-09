#include "common.h"

typedef unsigned char u8;

/* Widget-derived object: base constructor func_800953C0 (returns the object),
 * vtable pointer at +0x4C, id at +0x60, callback table at +0x64. */
typedef struct Widget8009E274 {
    u8 unk_00[0x4C];
    const u8 *vtable_4C;
    u8 unk_50[0x10];
    s32 id_60;
    const u8 *callbacks_64;
} Widget8009E274;

extern const u8 D_80151EC8[];
extern const u8 D_80152CB8[];
extern Widget8009E274 *func_800953C0(Widget8009E274 *self);

Widget8009E274 *func_8009E274(Widget8009E274 *self)
{
    func_800953C0(self);
    self->vtable_4C = D_80152CB8;
    self->id_60 = -1;
    self->callbacks_64 = D_80151EC8;
    return self;
}
