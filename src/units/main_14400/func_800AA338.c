#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 id; u8 pad1; u8 field_2; u8 field_3; u8 field_4; } State800AA338;
extern State800AA338 D_80142F24;
void func_800AC118(void);
void func_800ACF60(u8 id);
void func_800A9AD0(void);
void func_800AD004(u8 id);
void func_800AA338(s32 enable) {
    if (enable) {
        D_80142F24.field_2 = 0;
        D_80142F24.field_3 = 1;
        D_80142F24.field_4 = 0;
        func_800AC118();
        func_800ACF60(D_80142F24.id);
        if (D_80142F24.id != 20 && D_80142F24.id != 9 && D_80142F24.id != 10) {
            func_800A9AD0();
        }
    } else {
        func_800AD004(D_80142F24.id);
    }
}
