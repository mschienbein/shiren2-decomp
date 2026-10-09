#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
static inline unsigned char selection_flags(const SelectionRecord *record) { return record->flags; }

static inline unsigned char selection_mode(const SelectionRecord *record) { return record->mode; }


extern unsigned char D_801476D1;

extern signed char D_801476C2;

void func_80045A60(void);
void func_80045A84(void);
void func_80045E3C(void);

void func_800AA944(void) {
    s32 idle;

    if (D_801476D1 != 0) {
        return;
    }
    idle = ((selection_flags(&D_80142F18) >> 2) & 1) ^ 1;
    if (idle) {
        if (D_801476C2 == 0) {
            func_80045A60();
        }
        return;
    }
    switch (selection_mode(&D_80142F18)) {
    case 0x61:
    case 0x67:
    case 0x6A:
    case 0x6D:
        func_80045E3C();
        func_80045A60();
        break;
    default:
        func_80045A84();
        break;
    }
}
