#include "common.h"
/* Contained link at +8: func_800D0204 stores the actor's contained collection
 * (func_800EBA54) at +0 and its item (func_800E215C) at +4. */
typedef struct { void *collection; void *item; } Link;
typedef struct { short field_00; const void *field_04; Link field_08; } Object;
extern const s32 D_80157FA8[], D_80158388[12]; /* Whole 0x30-byte vtables. */
extern Link *func_800D0180(Link *);
extern void func_800D0204(Link *, void *actor);
Object *func_800DA970(Object *self, s32 kind, void *actor) {
    self->field_04 = &D_80157FA8;
    self->field_00 = kind;
    self->field_04 = &D_80158388;
    func_800D0180(&self->field_08);
    func_800D0204(&self->field_08, actor);
    return self;
}
