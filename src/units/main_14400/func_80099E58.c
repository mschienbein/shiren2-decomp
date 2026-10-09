#include "common.h"
typedef struct {
    unsigned char field_00[0x20]; s32 field_20;
    unsigned char field_24[0x10]; s32 field_34, field_38;
    unsigned char field_3c[0x294]; s32 field_2d0;
    signed char field_2d4[21]; unsigned char field_2e9[3]; s32 (*field_2ec)(void *);
} Object;
extern s32 func_80098E34(Object *, s32);
extern void *func_800980F0(Object *, s32);
extern s32 func_80099FFC(Object *, s32);
s32 func_80099E58(Object *self, s32 add) {
    s32 index = func_80098E34(self, self->field_34 + self->field_20 * self->field_38);
    s32 i;
    if (index < 0) return 0;
    if (self->field_2ec) {
        s32 rejected = self->field_2ec(func_800980F0(self, index));
        rejected ^= 1;
        if (rejected) return 0;
    }
    if (add) {
        s32 absent = func_80099FFC(self, index);
        s32 count;
        absent ^= 1;
        if (!absent) return 0;
        count = self->field_2d0;
        if (count < 21) {
            self->field_2d4[count] = index;
            self->field_2d0 = count + 1;
            return 1;
        }
    } else {
        for (i = 0; i < self->field_2d0; i++) {
            if (self->field_2d4[i] == index) {
                self->field_2d0--;
                for (; i < self->field_2d0; i++) self->field_2d4[i] = self->field_2d4[i + 1];
                return 1;
            }
        }
    }
    return 0;
}
