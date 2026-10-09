#include "common.h"

typedef unsigned char u8;

/* Widget-derived object: base constructor func_800953C0 (returns the object),
 * vtable pointer at +0x4C, id at +0x68, callback table at +0x6C. */
typedef struct Widget8009D76C {
    u8 unk_00[0x4C];
    const u8 *vtable_4C;
    u8 unk_50[0x18];
    s32 id_68;
    const u8 *callbacks_6C;
} Widget8009D76C;

extern const u8 D_80151EC8[];
extern const u8 D_80152AE8[];
extern Widget8009D76C *func_800953C0(Widget8009D76C *self);

Widget8009D76C *func_8009D76C(Widget8009D76C *self)
{
    func_800953C0(self);
    self->vtable_4C = D_80152AE8;
    self->id_68 = -1;
    self->callbacks_6C = D_80151EC8;
    return self;
}
