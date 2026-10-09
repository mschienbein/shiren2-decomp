#include "common.h"
typedef struct Obj800F68A4 {
    unsigned char pad_00[0x1E];
    unsigned char flags_1E;
    unsigned char pad_1F[0x39];
    struct Obj800F68A4 *target_58;
    unsigned char pad_5C[0x38];
    s32 field_94;
    s32 field_98;
    unsigned char field_9C;
} Obj800F68A4;
void func_800F68A4(Obj800F68A4 *self, Obj800F68A4 *other) {
    if (other != 0 && other != self) {
        if ((other->flags_1E >> 2) & 1) {
            self->field_94 = 1;
        } else if (self->field_94 == 0) {
            self->field_98 = 1;
            self->field_9C = 15;
            self->target_58 = other;
        }
    }
}
