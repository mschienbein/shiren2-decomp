#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
static inline unsigned char selection_kind(const SelectionRecord *record) { return record->kind; }

static inline unsigned char selection_variant(const SelectionRecord *record) { return record->variant; }


typedef unsigned char u8;
typedef signed char s8;



extern s8 D_801476C2;
extern s32 D_801F5D04;

void func_80072CE4(void);
void func_80048ABC(u32 value);
void func_80045DE8(void);
void func_800B2B50(void);
void func_80055B14(u32 word_00, u32 word_04);
void func_80046B08(s32 value);

void func_800C96FC(void) {
    if (D_801476C2 != 0) {
        func_80072CE4();
        func_80048ABC(1);
        func_80045DE8();
        D_801F5D04 = 1;
        func_800B2B50();
        func_80055B14(selection_variant(&D_80142F18), selection_kind(&D_80142F18));
        func_80046B08(D_801476C2);
    }
    D_801476C2 = 0;
}
