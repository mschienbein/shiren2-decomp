#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;



void func_800AC118(void);
void func_800ACF60(u8 id);
void func_800A9AD0(void);
void func_800AD004(u8 id);
void func_800AA338(s32 enable) {
    if (enable) {
        D_80142F24.previous = 0;
        D_80142F24.field_03 = 1;
        D_80142F24.masks[0] = 0;
        func_800AC118();
        func_800ACF60(D_80142F24.index);
        if (D_80142F24.index != 20 && D_80142F24.index != 9 && D_80142F24.index != 10) {
            func_800A9AD0();
        }
    } else {
        func_800AD004(D_80142F24.index);
    }
}
