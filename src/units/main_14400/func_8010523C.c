#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    char pad0[0x20];
    u32 field_20;
    char pad24[0x58];
    u16 field_7C;
    char pad7E[0x12];
    u32 field_90;
    char pad94[0xC];
    void *field_A0;
} S;
s32 func_800E0F40(S *s);
void func_8010523C(S *s) {
    s->field_90 &= ~0xA000;
    s->field_7C &= ~0x100;
    if (s->field_A0 != 0) {
        switch ((u8)func_800E0F40(s)) {
            case 2:
                s->field_7C |= 0x100;
                break;
            case 3:
                s->field_90 |= 0x8000;
                break;
            case 4:
                s->field_90 |= 0x2000;
                break;
        }
    }
    s->field_20 = s->field_90;
}
