#include "common.h"
typedef struct {
    unsigned char pad_00[0xD];
    unsigned char kind_0D;
    unsigned char variant_0E;
    unsigned char value_0F;
} Obj801284F8;
/* Partial view of the 0x18-byte record func_80044FDC loads into D_80160B30. */
typedef struct {
    unsigned char pad_00[0x12];
    unsigned char limit_12;
} Entry801284F8;
extern Entry801284F8 *func_80044FDC(unsigned char kind, unsigned char variant);
s32 func_801284F8(Obj801284F8 *self, s32 amount) {
    Entry801284F8 *info = func_80044FDC(self->kind_0D, self->variant_0E);
    u32 value = self->value_0F;
    u32 limit = info->limit_12;
    if (value < limit) {
        s32 new_value = value + amount;
        if ((s32)limit < new_value) {
            new_value = limit;
        }
        self->value_0F = new_value;
        return 1;
    }
    return 0;
}
