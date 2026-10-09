#include "common.h"
typedef struct { unsigned char pad0[4]; unsigned short state; unsigned short pad6; unsigned short phase; unsigned char padA[10]; s32 entity; s32 pad18; s32 timer; s32 pad20; s32 field_24, field_28, field_2C; } Task;
s32 func_80076044(s32, s32, s32, s32, s32);
void func_8008B8A8(s32 entity, s32 a, s32 b, s32 c);
void func_80087C20(Task *self) {
    switch (self->phase) {
    case 0:
        func_80076044(self->entity, 0x144, 2, 8, 1);
        self->timer = 12;
        self->phase++;
        break;
    case 1:
        if (self->timer-- == 0) {
            func_8008B8A8(self->entity, self->field_24, self->field_28, self->field_2C);
            self->state = 4;
        }
        break;
    }
}
