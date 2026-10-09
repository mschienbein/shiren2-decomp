#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern const unsigned char D_80142D18[12];
extern void func_800A9C8C(unsigned char, unsigned char);
void func_800A9DC0(unsigned char first, unsigned char second, s32 kind) {
    u32 index;
    func_800A9C8C(first, second);
    index = kind - 0x2D;
    if (index < 12) {
        D_80142F18.mode = D_80142D18[index];
        D_80142F18.coordinates[0] = -1;
        D_80142F18.coordinates[1] = -1;
        D_80142F18.kind = 0;
    }
}
