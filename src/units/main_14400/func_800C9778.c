#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionRecord D_80142F18;
extern SelectionSave D_80142F24;
typedef unsigned char u8;
/* Entire 0x30-byte record object at the established base, not field labels. */
typedef struct { u8 pad_00[7]; u8 field_07; u8 pad_08[0x14]; void *child_1C; u8 tail_20[0x10]; } Record;
Record D_80147680 = {0};
extern const u8 D_8015488C[8];
extern void func_800C9800(s32);
extern s32 func_800C9810(void);
extern void func_800C9820(s32, s32);
extern void func_800CADBC(Record *);
extern void func_800C9870(void);
static inline void mark_refresh(Record *record) {
    record->field_07 |= D_8015488C[1];
}
static inline u8 selection_index(SelectionSave *save) { return save->index; }
static inline u8 selection_count(SelectionSave *save) { return save->count; }
void func_800C9778(void) {
    if ((D_80142F18.flags >> 2) & 1) func_800C9800(1);
    if (func_800C9810()) {
        func_800C9820(selection_index(&D_80142F24), selection_count(&D_80142F24));
        mark_refresh(&D_80147680);
        func_800CADBC(&D_80147680);
        func_800C9870();
    }
}
