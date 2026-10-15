#include "func_801E8B10_13AA30/types.h"
extern S06Widget *func_800953C0(S06Widget *self);
extern void func_80097240(S06Widget *self, void *items, void *layout, void *selection);
extern const S06WidgetTable D_801F6060_147F80;
S06Widget *func_801E8B10_13AA30(S06Widget *self, void *items, void *layout, void *selection, s32 mode, s32 wrap) {
    func_800953C0(self);
    self->table4C = &D_801F6060_147F80;
    func_80097240(self, items, layout, selection);
    self->field5C = mode;
    self->field60 = wrap;
    return self;
}
