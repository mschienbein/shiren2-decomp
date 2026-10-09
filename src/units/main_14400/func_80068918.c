#include "common.h"

typedef unsigned char u8;

typedef struct { u8 kind; u8 sub; u8 flags; } Cell;
typedef struct {
    u8 pad_00[3];
    u8 red_03, green_04, blue_05;
    u8 weights_06[2];
    u8 weights_08[3];
    u8 weights_0B[3];
    u8 weights_0E[1];
    u8 weights_0F[1];
    u8 weights_10[3];
    u8 pad_13;
    u32 rom_start_14, rom_end_18;
    u8 *compressed_1C;
    u32 field_20;
} SceneEntry;
typedef struct { u8 packed; u8 variant; } CellStyle;

extern Cell *D_801E02A4;
extern u8 D_8016DC11;
extern SceneEntry D_8013C4FC[];
extern CellStyle D_8016DD24[4104];

void func_80061D28(s32 first, s32 second, s32 third, s32 fourth);
void func_800684E8(u8 x0, u8 y0, u8 x1, u8 y1);
void func_80069A00(s32 value);
u32 func_80069A10(void);

void func_80068918(s32 seed) {
    Cell *cell;
    CellStyle *style;
    u32 n;

    func_80061D28(10, 10, 0x41, 0x2B);
    func_800684E8(9, 9, 0x42, 0x2C);
    if (seed < 0) {
        return;
    }
    func_80069A00(seed);
    n = 0;
    cell = D_801E02A4;
    style = D_8016DD24;
    for (; n < 4104; n++, cell++, style++) {
        u32 pick[6];
        u32 i;
        for (i = 0; i < 6; i++) {
            u32 *dst;
            u32 count;
            u8 *weights;
            u32 roll;
            u32 sum;
            u32 j;
            switch (i) {
            case 0:
                dst = &pick[0];
                count = 2;
                weights = D_8013C4FC[D_8016DC11].weights_06;
                break;
            case 1:
                dst = &pick[1];
                count = 3;
                weights = D_8013C4FC[D_8016DC11].weights_08;
                break;
            case 2:
                dst = &pick[2];
                count = 3;
                weights = D_8013C4FC[D_8016DC11].weights_0B;
                break;
            case 3:
                dst = &pick[3];
                count = 1;
                weights = D_8013C4FC[D_8016DC11].weights_0E;
                break;
            case 4:
                dst = &pick[4];
                count = 1;
                weights = D_8013C4FC[D_8016DC11].weights_0F;
                break;
            case 5:
                dst = &pick[5];
                count = 3;
                weights = D_8013C4FC[D_8016DC11].weights_10;
                break;
            }
            sum = 0;
            roll = (s32)func_80069A10() % 100;
            for (j = 0; j < count; j++) {
                sum += weights[j];
                if (roll < sum) {
                    break;
                }
            }
            *dst = j;
        }
        if (cell->kind == 2) {
            pick[0] = 3;
        }
        style->packed = ((pick[0] & 3) << 6) | ((pick[1] & 3) << 4) | ((pick[2] & 3) << 2)
                      | ((pick[3] & 1) << 1) | (pick[4] & 1);
        style->variant = pick[5];
    }
}
