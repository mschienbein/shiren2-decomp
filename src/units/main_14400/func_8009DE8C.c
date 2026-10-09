#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x24]; s32 field_24; u8 pad_28[0x10]; s32 field_38; u8 pad_3C[0x15]; u8 field_51; } Save8009E724;
extern void func_8004898C(u8 *a, s32 value);
void func_8009DE8C(Save8009E724 *self) {
    s32 last = (self->field_24 - 1) / self->field_51;
    s32 current = self->field_38 / self->field_51;
    if (last > 0) {
        if (current == 0) last = 2;
        else if (current == last) last = 1;
        else last = 3;
        func_8004898C((u8 *)self, last);
    }
}
