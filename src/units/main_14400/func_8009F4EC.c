#include "common.h"

typedef unsigned char u8;

/* Widget-derived object: base constructor func_800953C0 (returns the object),
 * vtable pointer at +0x4C, id at +0x80, callback table at +0x94. */
typedef struct Widget8009F4EC {
    u8 unk_00[0x4C];
    const u8 *vtable_4C;
    u8 unk_50[0x30];
    s32 id_80;
    u8 unk_84[0x10];
    const u8 *callbacks_94;
} Widget8009F4EC;

extern const u8 D_80151EC8[];
extern const u8 D_80152F40[];
extern Widget8009F4EC *func_800953C0(Widget8009F4EC *self);

Widget8009F4EC *func_8009F4EC(Widget8009F4EC *self)
{
    func_800953C0(self);
    self->vtable_4C = D_80152F40;
    self->id_80 = -1;
    self->callbacks_94 = D_80151EC8;
    return self;
}
