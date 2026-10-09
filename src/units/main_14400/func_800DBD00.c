#include "common.h"
/* Collection/item link (func_800D01B8 shape). func_800DA8A0 copies the source link to +8;
 * the separate entry at +0x10 is the same kind of link (func_800DBF14 fills it through
 * func_800DAA58 for this class, D_80158568). */
typedef struct { void *collection; void *item; } Entry800D01B8;
struct VTable;
typedef struct { unsigned char pad0[4]; const struct VTable *field_4; unsigned char pad8[8]; Entry800D01B8 field_10; } Obj;
extern const struct VTable D_80158568;
void *func_800DA8A0(Obj *self, s32 kind, Entry800D01B8 *source);
Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);
Obj *func_800DBD00(Obj *self, Entry800D01B8 *source, Entry800D01B8 *entry) {
    func_800DA8A0(self, 0x15, source);
    self->field_4 = &D_80158568;
    func_800D0180(&self->field_10);
    self->field_10 = *entry;
    return self;
}
