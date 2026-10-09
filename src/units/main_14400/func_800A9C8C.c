#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 flag; u8 pad; u16 value; } Rec4;
typedef struct { u8 flag; u8 value; } Rec2;



extern Rec4 D_80142E88[];
extern Rec2 D_80142D28[];
extern u16 D_80142B14;
extern u16 D_80142B16;
void func_800ABBA0(u8 a, u8 b);
s32 func_800A99A8(void);
void func_800A9C8C(u8 a, u8 b) {
    s32 i;
    s32 other;
    D_80142F24.index = a;
    D_80142F24.count = b;
    func_800ABBA0(a, b);
    D_80142B14 = 0;
    for (i = 0; D_80142E88[i].flag != 0; i++) D_80142B14 += D_80142E88[i].value;
    D_80142B16 = 0;
    for (i = 0; D_80142D28[i].flag != 0; i++) D_80142B16 += D_80142D28[i].value;
    if (func_800A99A8() && D_80142F24.count == D_80142F24.previous_count) {
        D_80142F18.coordinates[0] = 7;
        D_80142F18.coordinates[1] = 7;
        D_80142F18.flags |= 0x50;
    }
    other = (D_80142F18.mode & 0xE0) != 0x40;
    if (other) D_80142F24.result = -1;
}
