#include "common.h"
typedef struct VTable VTable;
/* Collection/item link (func_800D01B8 shape); func_800DA8A0 copies the source link to +8. */
typedef struct { void *field_0; void *field_4; } Entry800D01B8;
typedef struct Obj { s32 field_00; const VTable *vtable_04; unsigned char pad_08[8]; Entry800D01B8 entry_10; } Obj;
extern const VTable D_80158628;
extern void *func_800DA8A0(Obj *obj, s32 kind, Entry800D01B8 *source);
extern Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);
Obj *func_800DC4C0(Obj *self, Entry800D01B8 *source, Entry800D01B8 *entry) {
    func_800DA8A0(self, 25, source);
    self->vtable_04 = &D_80158628;
    func_800D0180(&self->entry_10);
    self->entry_10 = *entry;
    return self;
}
