#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
extern void func_800D8F10(s32) __attribute__((noreturn));


void func_800AA5C0(s32 id) {
    id -= 0x18;
    if (id < 0) {
        func_800D8F10(0);
    }
    D_80142F24.masks[0] |= id;
}
