#include "common.h"
typedef struct { s32 field_0[3], field_C; } Object;
extern s32 D_80138CD0[];
extern void func_800826FC(s32);
extern void func_800827C8(s32);
void func_80048870(Object *self, s32 value) {
    if (self->field_C >= 0) {
        s32 direction;
        s32 code;
        func_800826FC(self->field_C);
        direction = value >> 25;
        if (direction == 0x39) code = 0x16;
        else { s32 group = direction / 8; code = direction % 8 != 4 ? D_80138CD0[group + 8] : D_80138CD0[group]; }
        func_800827C8(code);
    }
}
