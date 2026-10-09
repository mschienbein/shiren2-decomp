#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
struct Object;
extern struct Object D_80140160;
extern short D_80142B14, D_80142B16;

void func_80094B80(void *self, s32 value);
void func_800AA2D4(void) {
    D_80142B14 = 0;
    D_80142B16 = 0;
    D_80142F18.flags |= 4;
    func_80094B80(&D_80140160, 0);
}
