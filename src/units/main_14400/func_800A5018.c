#include "common.h"
typedef struct { unsigned char field_00[8]; unsigned char field_08; } Object;
extern signed char D_801429E8[][5];
extern void func_800A2F80(unsigned char *, s32);
extern s32 func_800A4EFC(Object *, unsigned char *);
s32 func_800A5018(Object *self, s32 kind) {
    unsigned char position;
    signed char *direction = D_801429E8[kind & 0xff];
    s32 i = 0;
    position = self->field_08;
    do {
        func_800A2F80(&position, *direction);
        if (func_800A4EFC(self, &position)) return 1;
        i++;
        direction++;
    } while (i < 5);
    return 0;
}
