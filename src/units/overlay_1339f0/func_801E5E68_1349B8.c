#include "func_801E5E68_1349B8/types.h"

static inline List801E5A30 *set_mode(List801E5A30 *list, s32 mode) {
    list->mode = mode;
    return list;
}

List801E5A30 *func_801E5E68_1349B8(Owner801E5E68 *self, s32 arg) {
    switch (self->methods->method_6C((u8 *)self + self->methods->delta_68, arg)) {
    case 1:
        return set_mode(&self->list64, self->methods->method_6C((u8 *)self + self->methods->delta_68, arg));
    case 2:
    case 3:
        return set_mode(&self->list384, self->methods->method_6C((u8 *)self + self->methods->delta_68, arg));
    }
    return 0;
}
