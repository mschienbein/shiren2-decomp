#include "common.h"
/* Item-set family D_80154300 slot +0x1C (D_80154390 binds func_800CE6F8): void (void *self, s32 value).
 * This wrapper forwards its incoming value unchanged (a1 at 0x800CEAA0). */
typedef struct {
    unsigned char pad_00[0x18];
    short adjustment_18;
    unsigned short reserved_1A;
    void (*set_1C)(void *self, s32 value);
} VTable800CEA88;
typedef struct { unsigned char pad_00[4]; VTable800CEA88 *vtable_04; } Obj800CEA88;
void func_800CEA88(Obj800CEA88 *self, s32 value) {
    VTable800CEA88 *table = self->vtable_04;
    void *adjusted = (unsigned char *)self + table->adjustment_18;
    table->set_1C(adjusted, value);
}
