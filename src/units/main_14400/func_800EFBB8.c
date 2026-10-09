#include "common.h"

/* 0x34-byte overlay secondary base at +0x78, constructed by func_801F2AC0 in
 * func_800EF930 (vtable word at +0x30 = obj+0xA8, D_80159300). */
typedef struct { unsigned char opaque[0x30]; void *vtable; } Sub801F2AC0;

typedef struct Obj800EFBB8 {
    unsigned char pad_00[0x78];
    Sub801F2AC0 base_78;
} Obj800EFBB8;

void func_800E032C(void *self, void *other);
/* Overlay method of the secondary base class. */
void func_801F2CB8(Sub801F2AC0 *base, void *other);

/* Virtual override: run the primary base method, then the secondary base method on the
 * converted (null-preserving) subobject pointer. */
void func_800EFBB8(Obj800EFBB8 *self, void *other)
{
    func_800E032C(self, other);
    func_801F2CB8(self != 0 ? &self->base_78 : 0, other);
}
