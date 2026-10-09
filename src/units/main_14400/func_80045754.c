#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
static inline unsigned char selection_flags(const SelectionRecord *record) { return record->flags; }

static inline signed char selection_status(const SelectionRecord *record) { return record->status; }


typedef unsigned char u8;
typedef signed char s8;

s32 D_80138BD4 = 1;
extern s32 D_80138BD8;



s32 func_80045CC0(void);
void func_80045A24(s32 sound);
void func_80045BE0(u8 a, u8 b);
s32 func_80046124(void);
s32 func_80045924(void);
void func_80045C4C(s32 value);

s32 func_80045754(s32 mode)
{
    s32 track = func_80045CC0();
    s32 special = 0;
    s32 changed;

    D_80138BD8 = selection_status(&D_80142F18);
    if (mode == 6 || mode == 8) {
        func_80045A24((selection_flags(&D_80142F18) & 3) == 1 ? 10 : 9);
    }
    func_80045BE0(0, 8);
    if (track == 0x46 || track == 0x47 || track == 0x1F || track == 0x20 || track == 0x22 ||
        track == 0x23 || track == 0x51 || track == 0x52 || track == 0x53 || track == 0x49 ||
        track == 0x3F || track == 0x4D) {
        special = 1;
    }
    if (D_80138BD8 == 0x44) {
        D_80138BD8 = func_80046124();
    }
    changed = func_80045924() != D_80138BD8 ? 1 : special;
    if (changed) {
        func_80045C4C(0x10);
        D_80138BD4 = 1;
    } else {
        D_80138BD4 = 0;
    }
    return D_80138BD8;
}
