#include "common.h"
typedef struct {
    unsigned char field_00[0x1e]; unsigned char field_1e;
    unsigned char field_1f[0x7b]; unsigned short field_9a;
} Object;
extern Object *func_800C5F60(void);
/* The virtual predicate supplies a receiver even though this override ignores it. */
s32 func_800F84D4(void *unused, Object *self, unsigned char *out) {
    s32 selected;
    *out = 0;
    if (!self) return 0;
    selected = 0;
    if (((self->field_1e >> 2) & 1) || self == func_800C5F60()) selected = 1;
    if (selected) return 1;
    if ((self->field_1e >> 3) & 1) return 1;
    if ((self->field_1e >> 4) & 1) return ((self->field_9a & 0x40) == 0) * 2;
    return 0;
}
